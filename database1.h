#ifndef DATABASE1_H
#define DATABASE1_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>
#include "list.h"
#include "tree.h"

class pointer_list;
class database2;

class pointer_list_node
{
private:
    pointer_list_node* next = nullptr;
    pointer_list_node* prev = nullptr;
    list_node* pointer = nullptr;
public:
    pointer_list_node() = default;
    ~pointer_list_node() { next = prev = nullptr; pointer = nullptr; }

    pointer_list_node* get_next() { return next; }
    pointer_list_node* get_prev() { return prev; }
    list_node* get_pointer() { return pointer; }
    friend class pointer_list;
};


class pointer_list
{
private:
    pointer_list_node* head = nullptr;
    list_node* if_action = nullptr;
public:
    pointer_list() = default;
    ~pointer_list() { delete_list(); head = nullptr; if_action = nullptr;}

    pointer_list_node* get_head() { return head; }
    void add_before_head(list_node *data);
    void add_head(list_node* data);

    void delete_list();

    void delete_node(pointer_list_node*);
    void killer(list_node*);

    list_node* delete_name_EQ(command& cmd);

    unsigned long int len();
};


class kilo_tree;

class database1//{tree + hash_array}
{
private:
    tree tr;
    pointer_list* pointer_li = nullptr;

    int p = 0;
    int group = 0;
    list_node* if_action = nullptr;
public:
    database1() = default;
    ~database1() { if (pointer_li) { delete[] pointer_li; pointer_li = nullptr; } p = 0; group = 0; if_action = nullptr; }

    void set_p(int value) { p = value; }    int get_p() { return p; }
    tree& get_tree() { return tr; }


    list_node* get_if_action() { return if_action; }
    int insert(list_node*);
    int big_insert(list_node*);
    int hash(const char* str);
    void init(list_node* node, int param);
    void big_init(list_node* node);

    friend class kilo_tree;
    friend class kilo_tree_node;

    void print(int depth = 0) { for (int i = 0; i < depth; i++) printf("  "); tr.print(); }

    void select_cond_NAME_EQ(command& cmd);

    list_node* delete_node_phone_EQ_and_name_EQ(command &cmd);
    list_node* big_delete_node_phone_EQ(command &cmd);
    list_node* delete_node_phone_EQ(command &cmd);
    list_node* delete_node_name_EQ(command &cmd);

    void killer(list_node* data_node);

    void how_good_is_hash();
};


class kilo_tree_node
{
private:
    kilo_tree_node* left = nullptr;
    kilo_tree_node* right = nullptr;
    int height = 1;
    database1* data = nullptr;
public:
    kilo_tree_node() = default;
    ~kilo_tree_node() { left = right = nullptr; }
    friend class kilo_tree;
    friend class database2;

    database1* get_data() { return data; }
    void init(list_node* data, int);
};


class kilo_tree//tree of {tree, hash-array}
{
private:
    kilo_tree_node* root = nullptr;
    kilo_tree_node* curr = nullptr;
    int p = 0;
    list_node* if_action = nullptr;
    database1* for_delete_group = nullptr;
public:
    int max_2 (int lhs, int rhs) { return lhs > rhs ? lhs : rhs; }

    void set_p(int value) { p = value; }    int get_p() { return p; }
    kilo_tree_node* get_root() { return root; }
    list_node* get_if_action() { return if_action; }
    kilo_tree() = default;
    ~kilo_tree() { delete_kilo_tree(); root = curr = nullptr; p = 0; if_action = nullptr; for_delete_group = nullptr; }

    void add_root(list_node* node);
    //tree_node* new_node(list_node* data);



    int get_height(kilo_tree_node *node);
    int get_balance(kilo_tree_node *node);

    kilo_tree_node *right_rotate(kilo_tree_node *a);
    kilo_tree_node *left_rotate(kilo_tree_node *a);

    kilo_tree_node* new_node(list_node* data);
    kilo_tree_node* insert(kilo_tree_node* node, list_node* data);
    list_node* insert(list_node* data);

    void delete_kilo_tree(kilo_tree_node* new_root);
    void delete_kilo_tree();

    void print_tree(kilo_tree_node* node, int depth);
    void print_tree();
    void print();

    void select_group_EQ(kilo_tree_node* node, command& cmd);
    void select_group_EQ(command& cmd);
    void select_group_LT(kilo_tree_node* node, command& cmd);
    void select_group_LT(command& cmd);

    void select_phone_EQ (kilo_tree_node* node, command &cmd);
    void select_phone_EQ (command &cmd);

    void select_name_EQ (kilo_tree_node* node, command &cmd);
    void select_name_EQ (command &cmd);
    void select_name_LIKE (kilo_tree_node* node, command &cmd);
    void select_name_LIKE (command &cmd);

    //--------
    void select_group_EQ_and_phone_EQ(kilo_tree_node* node, command& cmd);
    void select_group_EQ_and_phone_EQ(command& cmd);

    void select_group_EQ_and_phone_NEQ(kilo_tree_node* node, command& cmd);
    void select_group_EQ_and_phone_NEQ(command& cmd);

    void select_group_EQ_and_phone_GE(kilo_tree_node* node, command& cmd);
    void select_group_EQ_and_phone_GE(command& cmd);

    //------
    void select_group_EQ_and_name_EQ(kilo_tree_node* node, command& cmd);
    void select_group_EQ_and_name_EQ(command& cmd);

    void select_phone_EQ_and_name_EQ(kilo_tree_node* node, command& cmd);
    void select_phone_EQ_and_name_EQ(command& cmd);
    //---

    void delete_node_group_EQ_and_phone_EQ(kilo_tree_node* node, command& cmd);
    void delete_node_group_EQ_and_phone_EQ(command& cmd);

    void delete_node_group_EQ(kilo_tree_node* node, command& cmd);
    void delete_node_group_EQ(command& cmd);
//

    void delete_node_group_EQ_and_name_EQ(kilo_tree_node* node, command& cmd);
    void delete_node_group_EQ_and_name_EQ(command& cmd);

    void killer(kilo_tree_node* node, list_node* data_node);
    void killer(list_node* data_node);
};

#endif //DATABASE1_H
