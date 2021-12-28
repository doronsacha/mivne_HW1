#ifndef AVL_TREE_H
#define AVL_TREE_H
//Created By Tsahi and Doron
#include <memory>
#include <iostream>

using namespace std;

template <class T,class S>
class AVL{
private:
    class Node{
    public:
        Node* right;
        Node* left;
        T key;
        S data;
    };
    Node *root;
    int size;
public:
    /**Basic Avl-Tree Constructor, initialize an empty root node. */
    AVL() : root(nullptr),size(0) {}

    /** Destructor. */
    ~AVL(){
        treeClear();
    }

    /** Pre-order traversal. */
    void preorder(){
        preorderAid(root);
    }

    /** Post-order traversal. */
    void postorder(){
        postorderAid(root);
    }

    /** In-order traversal. */
        void inorder(){
            inorderAid(root);
        }


    /** Inserting a new node with the given key. may required some rotating. */
    void insert(T key,S data)
    {
        root = insertUtil(root,key,data);
        size++;
    }

    /** A function used to remove a specific node with the given key. */
    void remove(T key)
    {
        root = removeAid(root, key);
        size--;
    }

    /** A function that completely removes all the nodes in the tree, and set it's root to null.*/
    void treeClear(){
            treeClearAid(root);
            root = nullptr;
            size=0;

    }

    /** A function used in order to get the node's Data.*/
    T getData(Node* v) const
    {
        return v->data;
    }
    Node* findNode(T key)
    {
        return findNodeAid(root,key);
    }
    Node * find_max_root()
    {
        return findMax(root);
    }
    Node * getRoot() const
    {
        return root;
    }
    int getSize() const
    {
        return size;
    }


//**********************************************************************************************************************
//----------------------------------------------Balancing---------------------------------------------------------------
//**********************************************************************************************************************

    /** balance the tree using the AVL rules for balance factor that doesn't belong to [-1,1].*/
    Node* balance(Node *t) {
        int bal_factor = getBalanceFactor(t);
        if (bal_factor > 1) {
            if (getBalanceFactor(t->left) > 0)
                t = ll_rotate(t);
            else
                t = lr_rotate(t);
        } else if (bal_factor < -1) {
            if (getBalanceFactor(t->right) > 0)
                t = rl_rotate(t);
            else
                t = rr_rotate(t);
        }
        return t;
    }

    /** A function used in order to get the node's height. */
    int height(Node* v) const{
        int h =0;
        if (v != nullptr)
        {
            int l_height = height(v->left);
            int r_height = height(v->right);
            int max_height = (l_height > r_height) ? l_height : r_height;
            h = max_height + 1;
        }
        return h;
    }

    /** calculating node's balance factor.*/
    int getBalanceFactor(Node *t){
        int l_height = height(t->left);
        int r_height = height(t->right);
        int b_factor = l_height - r_height;
        return b_factor;
    }

//**********************************************************************************************************************
//----------------------------------------------Rotating----------------------------------------------------------------
//**********************************************************************************************************************

    Node * rr_rotate(Node *parent){
        Node *t;
        t = parent->right;
        parent->right = t->left;
        t->left = parent;
        return t;
    }

    Node * ll_rotate(Node *parent){
        Node *t;
        t = parent->left;
        parent->left = t->right;
        t->right = parent;
        return t;
    }

    Node * lr_rotate(Node *parent){
        Node *t;
        t = parent->left;
        parent->left = rr_rotate(t);
        return ll_rotate(parent);
    }

    Node * rl_rotate(Node *parent){
        Node *t;
        t = parent->right;
        parent->right = ll_rotate(t);
        return rr_rotate(parent);
    }

//**********************************************************************************************************************
//----------------------------------------------Searching---------------------------------------------------------------
//**********************************************************************************************************************

    /** A function used in order to find the minimum of the tree. */
    Node* findMin(Node * v) const
    {
        if(v == nullptr)
            return nullptr;
        else if(v->left == nullptr)
            return v;
        else
            return findMin(v->left);
    }

    /** A function used in order to find the maximum of the tree. */
    Node* findMax(Node * v) const
    {
        if(v == nullptr)
            return nullptr;
        else if(v->right == nullptr)
            return v;
        else
            return findMax(v->right);
    }

    /** A function used in order to search a specific node on the tree,given its key. */
    Node* findNodeAid(Node *v,T &key) const
    {
        if (v == nullptr) return nullptr;
        if(v->key == key) return v;
        else if (v->key < key)
        {
            return findNodeAid(v->right, key);
        }
        else{
            return findNodeAid(v->left,key);
        }
    }

//**********************************************************************************************************************
//----------------------------------------------Aid functions---------------------------------------------------------------
//**********************************************************************************************************************

    void preorderAid(Node* r){
        if (r == nullptr)
            return;
        cout << r->data << "  ";
        preorderAid(r->left);
        preorderAid(r->right);
    }

    void postorderAid(Node* r){
        if (r == nullptr)
            return;
        postorderAid(r->left);
        postorderAid(r->right);
        cout << r->data << "  ";
    }

    void inorderAid(Node* r){
        if (r == nullptr)
            return;
        inorderAid(r->left);
        cout << r->data << "  ";
        inorderAid(r->right);
    }

    Node* insertUtil(Node * r, T key,S data)
    {
        if (r == nullptr) {
            r = new Node;
            r->key = key;
            r->data = data;
            r->left = nullptr;
            r->right = nullptr;
            return r;
        }
        else if (key < r->key)
        {
            r->left = insertUtil(r->left, key,data);
            r = balance(r);
        } else if (r->key <= key) {
            r->right = insertUtil(r->right, key, data);
            r = balance(r);
        } return r;
    }

    Node* removeAid(Node* t, T key) {
        Node* temp;
        // element not found
        if (t == nullptr) return nullptr;
            // searching element
        else if (key < t->key) t->left = removeAid(t->left, key);
        else if (t->key < key) t->right = removeAid(t->right, key);
            // element found
            // element has 2 children
        else if (t->left && t->right) {
            temp = findMin(t->right);
            t->key = temp->key;
            t->data=temp->data;
            t->right = removeAid(t->right, t->key);
        }
            // if element has 1 or 0 child
        else {
            temp = t;
            if (t->left == nullptr) t = t->right;
            else if (t->right == nullptr) t = t->left;
            delete temp;
        }
        if (t == nullptr) return t;
        // check balanced
        t = balance(t);
        return t;
    }

    void treeClearAid(Node* v){
        if(v != nullptr){
            treeClearAid(v->left);
            treeClearAid(v->right);
            delete v;
        }
    }

    void transform_to_array_by_order_helper(Node* firstNode,S **arr, int* i)
    {
        if(firstNode== nullptr)
        {
            return;
        }
        transform_to_array_by_order_helper(firstNode->right,arr,i);
        (*arr)[*i]=firstNode->data;
        (*i)++;
        transform_to_array_by_order_helper(firstNode->left,arr,i);
    }
    void transform_to_array_by_order(S **arr, int* i)
    {
        transform_to_array_by_order_helper(root,arr,i);
    }

    void transform_to_array_by_inorder_helper(Node* firstNode,S **arr, int* i)
    {
        if(firstNode== nullptr)
        {
            return;
        }
        transform_to_array_by_inorder_helper(firstNode->left,arr,i);
        (*arr)[*i]=firstNode->data;
        (*i)++;
        transform_to_array_by_inorder_helper(firstNode->right,arr,i);
    }
    void transform_to_array_by_inorder(S **arr, int* i)
    {
        transform_to_array_by_inorder_helper(root,arr,i);
    }

    // for keys.
    void transform_to_array_by_inorder_keys(T **arr, int* i)
    {
        transform_to_array_by_inorder_helper_keys(root,arr,i);
    }

    void transform_to_array_by_inorder_helper_keys(Node* firstNode,T **arr, int* i)
    {
        if(firstNode== nullptr)
        {
            return;
        }
        transform_to_array_by_inorder_helper_keys(firstNode->left,arr,i);
        (*arr)[*i]=firstNode->key;
        (*i)++;
        transform_to_array_by_inorder_helper_keys(firstNode->right,arr,i);
    }



    void fill_tree_helper(Node* firstNode,T ** arrKey,S **arrData, int* i)
    {
        if(firstNode== nullptr)
        {
            return;
        }
        fill_tree_helper(firstNode->left,arrKey,arrData,i);
        firstNode->key = (*arrKey)[*i];
        firstNode->data = (*arrData)[*i];
        (*i)++;
        fill_tree_helper(firstNode->right,arrKey,arrData,i);
    }

    void fill_tree(T ** arrKey,S **arrData, int* i)
    {
        fill_tree_helper(root,arrKey,arrData,i);
    }



    void treeDataClear(){
        treeDataClearAid(root);
        root = nullptr;
        size=0;
    }

    void treeDataClearAid(Node* v){
        if(v != nullptr){
            treeDataClearAid(v->left);
            treeDataClearAid(v->right);
            delete v->data;
            delete v;
        }
    }

    void treeSpecialDataClear(){
        treeSpecialDataClearAid(root);
        root = nullptr;
        size=0;
    }

    void treeSpecialDataClearAid(Node* v){
        if(v != nullptr){
            treeSpecialDataClearAid(v->left);
            treeSpecialDataClearAid(v->right);
            delete v->data.player;
            delete v;
        }
    }

    void transform_to_array_by_inorder_maximum_n(S **arr, int* i,int n)
    {
        transform_to_array_by_inorder_maximum_n_helper(root,arr,i,n);
    }

    void transform_to_array_by_inorder_maximum_n_helper(Node* firstNode,S **arr, int* i,int n)
    {
        if(firstNode== nullptr)
        {
            return;
        }
        transform_to_array_by_inorder_maximum_n_helper(firstNode->left,arr,i,n);
        if(*i >n-1)
        {
            return;
        }
        (*arr)[*i]=firstNode->data;
        (*i)++;
        transform_to_array_by_inorder_maximum_n_helper(firstNode->right,arr,i,n);
    }


    
};

#endif //AVL_TREE_H