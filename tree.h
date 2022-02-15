#ifndef TREE_H
#define TREE_H

#include "razbor.h"
#include "list.h"

static int schet = 0;

class tree;
//class list;

class tree_node
{
private:
    tree_node *left = nullptr;
    tree_node *right = nullptr;
    int height = 1;
    list_node *data = nullptr;

    //tree_node *next_found = nullptr;
public:
    friend class tree;

    tree_node() = default;
    ~tree_node() { left = right = nullptr; height = 1; data = nullptr; }

    void set_left(tree_node *left)
    { this->left = left; }
    void set_right(tree_node *right)
    { this->right = right; }
    tree_node* get_left() const
    { return left; }
    tree_node* get_right() const
    { return right; }
    int get_height() const
    { return height; }
    list_node* get_data() const
    { return data; }
};




class tree
{
private:
    tree_node *root = nullptr;
    tree_node *curr = nullptr;

    bool has_deleted = 0;
    list_node* if_action = nullptr;

public:
    list_node* get_if_action() const { return if_action; }

    int max_2 (int lhs, int rhs) { return lhs > rhs ? lhs : rhs; }

    void add_root(list_node* node) //adds root if the tree is empty
    {
        root = new tree_node();
        root->data = node;
    }

    tree_node* new_node(list_node* data)
    {
        if_action = data;
        tree_node* node = new tree_node();
        node->data = data;
        //li.add_before_head(data);
        if (!root) root = node;

        return node;
    }

    int get_height(tree_node *node)
    {
        if (!node)
            return 0;
        return node->height;
    }

    int get_balance(tree_node *node)
    {
        if (!node) return 0;
        return get_height(node->left) - get_height(node->right);
    }

    tree_node *right_rotate(tree_node *a)
    {
        tree_node *b = a->left;
        tree_node *c = b->right;

        //preserve our roots
        if (a == root) root = b;

        //rotate it
        b->right = a;
        a->left = c;

        //change heights. BOTH.
        a->height = 1 + max_2(get_height(a->left), get_height(a->right));
        b->height = 1 + max_2(get_height(b->left), get_height(b->right));

        return b;
    }


    tree_node *left_rotate(tree_node *a)
    {
        tree_node *b = a->right;
        tree_node *c = b->left;

        //preserve our roots
        if (a == root) root = b;

        //rotate it
        b->left = a;
        a->right = c;

        //change heights. BOTH.
        a->height = 1 + max_2(get_height(a->left), get_height(a->right));
        b->height = 1 + max_2(get_height(b->left), get_height(b->right));

        return b;
    }




    tree_node* insert(tree_node* node, list_node* data)
    {   //yeah, data was not a good name

        if (!node)
            return(new_node(data));

        if (data->spec_cmp_phone_lt(*node->data))//(data < node->data)
            node->left = insert(node->left, data);
        else if (data->spec_cmp_phone_gt(*node->data))//(data > node->data)
            node->right = insert(node->right, data);
            else //they two are the same => no need for double
            {
                delete data;
                if_action = nullptr;
                return node;
            }
        //change height of ancestor
        node->height = 1 + max_2(get_height(node->left), get_height(node->right));

        int balance = get_balance(node);

        //if ~balanced
        if (!balance || balance == -1 || balance == 1) return node;


            //if not balanced
        //L L Case
        if (balance > 1 && //data < node->left->data
            data->spec_cmp_phone_lt(*node->left->data))
        {
            //printf("1\n");
            return right_rotate(node);
        }
        //R R Case
        if (balance < -1 && //data > node->right->data
            data->spec_cmp_phone_gt(*node->right->data))
        {
            //printf("2\n");
            return left_rotate(node);
        }
        // L R Case
        if (balance > 1 && //data > node->left->data
            data->spec_cmp_phone_gt(*node->left->data))
        {
            node->left = left_rotate(node->left);
            //printf("3\n");
            return right_rotate(node);
        }

        // R L Case
        if (balance < -1 && //data < node->right->data
            data->spec_cmp_phone_lt(*node->right->data))
        {
            node->right = right_rotate(node->right);
            //printf("4\n");
            return left_rotate(node);
        }

        // return the (unchanged) node pointer
        printf("HOW\n");
        return node;
    }

    list_node* insert(list_node *data)
    {
        if_action = nullptr;
        insert(root, data);
        return if_action;
    }


    tree_node* big_insert(tree_node* node, list_node* data)
    {   //yeah, data was not a good name

        if (!node)
            return(new_node(data));

        if (data->big_spec_cmp_phone_lt(*node->data))//(data < node->data)
            node->left = big_insert(node->left, data);
        else if (data->big_spec_cmp_phone_gt(*node->data))//(data > node->data)
            node->right = big_insert(node->right, data);
        else //they two are the same => no need for double
        {
            delete data;
            if_action = nullptr;
            return node;
        }
        //change height of ancestor
        node->height = 1 + max_2(get_height(node->left), get_height(node->right));

        int balance = get_balance(node);

        //if ~balanced
        if (!balance || balance == -1 || balance == 1) return node;


        //if not balanced
        //L L Case
        if (balance > 1 && //data < node->left->data
            data->big_spec_cmp_phone_lt(*node->left->data))
        {
            //printf("1\n");
            return right_rotate(node);
        }
        //R R Case
        if (balance < -1 && //data > node->right->data
            data->big_spec_cmp_phone_gt(*node->right->data))
        {
            //printf("2\n");
            return left_rotate(node);
        }
        // L R Case
        if (balance > 1 && //data > node->left->data
            data->big_spec_cmp_phone_gt(*node->left->data))
        {
            node->left = left_rotate(node->left);
            //printf("3\n");
            return right_rotate(node);
        }

        // R L Case
        if (balance < -1 && //data < node->right->data
            data->big_spec_cmp_phone_lt(*node->right->data))
        {
            node->right = right_rotate(node->right);
            //printf("4\n");
            return left_rotate(node);
        }

        // return the (unchanged) node pointer
        printf("HOW\n");
        return node;
    }

    list_node* big_insert(list_node *data)
    {
        if_action = nullptr;
        big_insert(root, data);
        return if_action;
    }


    void delete_tree(tree_node* new_root)
    {
        if (!new_root) return;
        if (new_root->left)
        {
            delete_tree(new_root->left); //nullify pointers
            new_root->left = nullptr;  //I guess it nullifies
        }
        if (new_root->right)
        {
            delete_tree(new_root->right);
            new_root->right = nullptr;
        }

        delete new_root;
    }

    void delete_tree()
    {
        delete_tree(root);
        curr = root = nullptr; has_deleted = 0; if_action = nullptr;
    }

    tree() = default;
    ~tree() { delete_tree(); }

    tree_node * find_min(tree_node* node) //yes, it finds min in subtree
    {                                          //works
        curr = node;

        //leftest node
        while (curr->left != NULL)
            curr = curr->left;

        return curr;
    }

    tree_node* balansirovka(tree_node* node)
    {
        if (!node)
            return nullptr;

        node->height = 1 + max_2(get_height(node->left), get_height(node->right)); //change height

        int balance = get_balance(node);

        if (!balance || balance == -1 || balance == 1) return node;

        if (balance > 1)//4 cases, now they are non-linear
        {
            if (get_balance(node->left) >= 0) //L L
                return right_rotate(node);
            node->left = left_rotate(node->left);// if get_balance() < 0 aka L R
            return right_rotate(node);
        }

        if (balance < -1) //TODO
        {
            if (get_balance(node->right) <= 0)
                return left_rotate(node);
            node->right = right_rotate(node->right);// >0
            return left_rotate(node);
        }

        printf("how did it happen\n");
        return node;
    }

    tree_node* killer(tree_node* node, list_node* data)
    {
        //data was not the best choice indeed

        if (!node)
            return nullptr;

        if (data->spec_cmp_phone_lt(*node->data)) // data.data < *(node->data)
            node->left = killer(node->left, data);
        else if (data->spec_cmp_phone_gt(*node->data))//data.data > *(node->data)
            node->right = killer(node->right, data);

        else //equality => delete
        {
            //node with <=1 child
            if(!node->left || !node->right)
            {
                tree_node *tmp = (node->left) ? (node->left) : (node->right);

                if (!tmp)//zero child case
                {
                    //printf("second\n"); print_tree(node, 0);
                    //printf("third\n"); print_tree()
                    if (node == root) root = nullptr;
                    delete node;
                    has_deleted = 1;
                    return nullptr;
                    //printf("fifth\n"); print_tree();
                }
                else //one child of a node
                {
                    if (node == root) root = tmp;
                    delete node;
                    has_deleted = 1;
                    return tmp;
                }

                //IF_ACTION HERE IS PROHIBITED


                printf("Uh-uh, not today\n"); //never triggers
                //printf("fourth\n"); print_tree();
            }
            else
            {
                //printf("first\n"); print_tree();
                //printf("3\n");

                //node with two children:
                tree_node* tmp = find_min(node->right);

                //copy data

                node->data = tmp->data;
                //printf("first + 1 / 2\n"); print_tree();
                has_deleted = 1;
                node->right = killer(node->right, tmp->data);
            }
        }

        return balansirovka(node);
    }

     tree_node* big_killer(tree_node* node, list_node* data)
    {
        //data was not the best choice indeed

        if (!node)
            return nullptr;

        if (data->big_spec_cmp_phone_lt(*node->data)) // data.data < *(node->data)
            node->left = big_killer(node->left, data);
        else if (data->big_spec_cmp_phone_gt(*node->data))//data.data > *(node->data)
            node->right = big_killer(node->right, data);

        else //equality => delete
        {
            //node with <=1 child
            if(!node->left || !node->right)
            {
                tree_node *tmp = (node->left) ? (node->left) : (node->right);

                if (!tmp)//zero child case
                {
                    //printf("second\n"); print_tree(node, 0);
                    //printf("third\n"); print_tree()
                    if (node == root) root = nullptr;
                    delete node;
                    has_deleted = 1;
                    return nullptr;
                    //printf("fifth\n"); print_tree();
                }
                else //one child of a node
                {
                    if (node == root) root = tmp;
                    delete node;
                    has_deleted = 1;
                    return tmp;
                }

                //IF_ACTION HERE IS PROHIBITED


                printf("Uh-uh, not today\n"); //never triggers
                //printf("fourth\n"); print_tree();
            }
            else
            {
                //printf("first\n"); print_tree();
                //printf("3\n");

                //node with two children:
                tree_node* tmp = find_min(node->right);

                //copy data

                node->data = tmp->data;
                //printf("first + 1 / 2\n"); print_tree();
                has_deleted = 1;
                node->right = big_killer(node->right, tmp->data);
            }
        }

        return balansirovka(node);
    }

    tree_node* delete_node_phone_EQ(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ(node->right, cmd);

        else//equality => delete
        {
            //node with <=1 child
            if(!node->left || !node->right)
            {
                tree_node *tmp = (node->left) ? (node->left) : (node->right);

                //zero child case
                if (!tmp)
                {
                    //printf("second\n"); print_tree(node, 0);
                    //
                    //printf("third\n"); print_tree();
                    if (node == root) root = nullptr;
                    if_action = node->data;//new
                    delete node;
                    has_deleted = 1;
                    return nullptr;
                    //printf("fifth\n"); print_tree();
                }
                else //one child of a node
                {
                    //
                    if (node == root) root = tmp;
                    if_action = node->data;//new
                    delete node;
                    has_deleted = 1;
                    return tmp;
                }
                //printf("fourth\n"); print_tree();
            }
            else
            {
                //printf("first\n"); print_tree();
                //node with two children:
                tree_node* tmp = find_min(node->right);

                if_action = node->data;//new
                //copy data
                node->data = tmp->data;
                //printf("first + 1 / 2\n"); print_tree();
                has_deleted = 1;

                node->right = killer(node->right, tmp->data);
            }
            //node = delete_node_phone_EQ(node, cmd);
        }

        return balansirovka(node);
    }


    tree_node* big_delete_node_phone_EQ(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ(node->right, cmd);

        else//equality => delete
        {
            //node with <=1 child
            if(!node->left || !node->right)
            {
                tree_node *tmp = (node->left) ? (node->left) : (node->right);

                //zero child case
                if (!tmp)
                {
                    //printf("second\n"); print_tree(node, 0);
                    //printf("third\n"); print_tree();
                    if (node == root) root = nullptr;
                    if_action = node->data;//new
                    delete node;
                    has_deleted = 1;
                    return nullptr;
                    //printf("fifth\n"); print_tree();
                }
                else //one child of a node
                {
                    //
                    if (node == root) root = tmp;
                    if_action = node->data;//new
                    delete node;
                    has_deleted = 1;
                    return tmp;
                }
                //printf("fourth\n"); print_tree();
            }
            else
            {
                //printf("first\n"); print_tree();
                //node with two children:
                tree_node* tmp = find_min(node->right);

                if_action = node->data;//new
                //copy data
                node->data = tmp->data;
                has_deleted = 1;

                node->right = big_killer(node->right, tmp->data);
            }
        }

        return balansirovka(node);
    }



    tree_node* delete_node_phone_EQ(command& cmd)
    {
        if_action = nullptr;
        return delete_node_phone_EQ(root, cmd);
    }


    tree_node* big_delete_node_phone_EQ(command& cmd)
    {
        if_action = nullptr;
        return big_delete_node_phone_EQ(root, cmd);
    }


    //already not FOR TESTS ONLY
    tree_node* killer(list_node* data)
    {
        return killer(root, data);
    }

    tree_node* big_killer(list_node* data)
    {
        return big_killer(root, data);
    }

    tree_node* killer_if_action(tree_node* node, list_node* data) //flag = 1 -> we are not doing li.delete_node()
    {
        //data was not the best choice indeed

        if (!node)
            return nullptr;

        if (data->spec_cmp_phone_lt(*node->data)) // data.data < *(node->data)
            node->left = killer_if_action(node->left, data);
        else if (data->spec_cmp_phone_gt(*node->data))//data.data > *(node->data)
            node->right = killer_if_action(node->right, data);

        else //equality => delete
        {
            //node with <=1 child
            if(!node->left || !node->right)
            {
                tree_node *tmp = (node->left) ? (node->left) : (node->right);

                if (!tmp)//zero child case
                {
                    //printf("second\n"); print_tree(node, 0);
                    //printf("third\n"); print_tree()
                    if (node == root) root = nullptr;
                    if_action = node->data;
                    delete node;
                    has_deleted = 1;
                    return nullptr;
                    //printf("fifth\n"); print_tree();
                }
                else //one child of a node
                {
                    if (node == root) root = tmp;
                    if_action = node->data;
                    delete node;
                    has_deleted = 1;
                    return tmp;
                }

                //IF_ACTION HERE IS PROHIBITED


                printf("Uh-uh, not today\n"); //never triggers
                //printf("fourth\n"); print_tree();
            }
            else
            {
                //printf("first\n"); print_tree();
                //printf("3\n");

                //node with two children:
                tree_node* tmp = find_min(node->right);

                if_action = node->data;
                node->data = tmp->data;
                //printf("first + 1 / 2\n"); print_tree();
                has_deleted = 1;
                node->right = killer_if_action(node->right, tmp->data);
            }
        }

        return balansirovka(node);
    }
    tree_node* killer_if_action() { if_action = nullptr; return killer_if_action(root, root->data); }


    tree_node* delete_node_phone_EQ_and_group_EQ(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_group_EQ(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_group_EQ(node->right, cmd);

        else//equality => delete
        {
            if (cmd.group < node->data->group)
                node->left = delete_node_phone_EQ_and_group_EQ(node->left, cmd);
            else if (cmd.group > node->data->group) //cmd.data > *(node->data)
                node->right = delete_node_phone_EQ_and_group_EQ(node->right, cmd);
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        if_action = node->data;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = tmp;
                        if_action = node->data;
                        delete node;
                        has_deleted = 1;
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    if_action = node->data;
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }
                //node = balansirovka(node);
                //node = delete_node_phone_EQ_and_group_EQ(node, cmd);
            }
        }

        return balansirovka(node);
    }

    tree_node* delete_node_phone_EQ_and_name_EQ(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_name_EQ(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_name_EQ(node->right, cmd);

        else//equality => delete
        {
            if (cmd.cmp_NAME_GT(node->data))
                node->left = delete_node_phone_EQ_and_name_EQ(node->left, cmd);
            else if (cmd.cmp_NAME_LT(node->data)) //cmd.data > *(node->data)
                node->right = delete_node_phone_EQ_and_name_EQ(node->right, cmd);
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        if_action = node->data;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = tmp;
                        if_action = node->data;
                        delete node;
                        has_deleted = 1;
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    if_action = node->data;
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }
                //node = balansirovka(node);
                //node = delete_node_phone_EQ_and_name_EQ(node, cmd);
            }
        }

        return balansirovka(node);
    }

    void delete_node_phone_EQ_and_name_EQ(command& cmd)
    {
        if_action = nullptr;
        delete_node_phone_EQ_and_name_EQ(root, cmd);
    }

    //---
    tree_node* delete_node_phone_EQ_and_group_NEQ(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_group_NEQ(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_group_NEQ(node->right, cmd);

        else//equality => delete
        {
            if (cmd.group == node->data->group)
            {
                node->left = delete_node_phone_EQ_and_group_NEQ(node->left, cmd);
                node = balansirovka(node);
                node->right = delete_node_phone_EQ_and_group_NEQ(node->right, cmd);
            }
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = tmp;
                        delete node;
                        has_deleted = 1;
                        tmp = delete_node_phone_EQ_and_group_NEQ(tmp, cmd);
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    //copy data
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }

                node = balansirovka(node);
            }
        }

        return balansirovka(node);
    }

    void delete_phone_EQ_and_group_NEQ(command& cmd)
    {
        delete_node_phone_EQ_and_group_NEQ(root, cmd);
        while(has_deleted)
        {
            has_deleted = 0;
            delete_node_phone_EQ_and_group_NEQ(root, cmd);
        }
    }

    tree_node* delete_node_phone_EQ_and_group_LT(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_group_LT(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_group_LT(node->right, cmd);

        else//equality => delete
        {
            if (cmd.group <= node->data->group)
            {
                node->left = delete_node_phone_EQ_and_group_LT(node->left, cmd);
            }
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = node;
                        delete node;
                        has_deleted = 1;
                        tmp = delete_node_phone_EQ_and_group_LT(tmp, cmd);
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    //copy data
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }

                node = balansirovka(node);
            }
        }

        return balansirovka(node);
    }

    void delete_phone_EQ_and_group_LT(command& cmd)
    {
        delete_node_phone_EQ_and_group_LT(root, cmd);
        while(has_deleted)
        {
            has_deleted = 0;
            delete_node_phone_EQ_and_group_LT(root, cmd);
        }
    }

    //--------


    tree_node* delete_node_phone_EQ_and_group_GT(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_group_GT(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_group_GT(node->right, cmd);

        else//equality => delete
        {
            if (cmd.group >= node->data->group)
            {
                node->right = delete_node_phone_EQ_and_group_GT(node->right, cmd);
            }
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = node;
                        delete node;
                        has_deleted = 1;
                        tmp = delete_node_phone_EQ_and_group_GT(tmp, cmd);
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    //copy data
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }

                node = balansirovka(node);
            }
        }

        return balansirovka(node);
    }

    void delete_phone_EQ_and_group_GT(command& cmd)
    {
        delete_node_phone_EQ_and_group_GT(root, cmd);
        while(has_deleted)
        {
            has_deleted = 0;
            delete_node_phone_EQ_and_group_GT(root, cmd);
        }
    }

    tree_node* delete_node_phone_EQ_and_group_LE(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_group_LE(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_group_LE(node->right, cmd);

        else//equality => delete
        {
            if (cmd.group < node->data->group)
            {
                node->left = delete_node_phone_EQ_and_group_LE(node->left, cmd);
            }
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = node;
                        delete node;
                        has_deleted = 1;
                        tmp = delete_node_phone_EQ_and_group_LE(tmp, cmd);
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    //copy data
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }

                node = balansirovka(node);
            }
        }

        return balansirovka(node);
    }

    void delete_phone_EQ_and_group_LE(command& cmd)
    {
        delete_node_phone_EQ_and_group_LE(root, cmd);
        while(has_deleted)
        {
            has_deleted = 0;
            delete_node_phone_EQ_and_group_LE(root, cmd);
        }
    }

    tree_node* delete_node_phone_EQ_and_group_GE(tree_node* node, command& cmd)
    {
        if (!node)
            return nullptr;

        if (cmd.phone < node->data->phone) // cmd.data < *(node->data)
            node->left = delete_node_phone_EQ_and_group_GE(node->left, cmd);
        else if (cmd.phone > node->data->phone) //cmd.data > *(node->data)
            node->right = delete_node_phone_EQ_and_group_GE(node->right, cmd);

        else//equality => delete
        {
            if (cmd.group > node->data->group)
            {
                node->right = delete_node_phone_EQ_and_group_GE(node->right, cmd);
            }
            else
            {
                if(!node->left || !node->right)//node with <=1 child
                {
                    tree_node *tmp = (node->left) ? (node->left) : (node->right);

                    //zero child case
                    if (!tmp)
                    {
                        //printf("second\n"); print_tree(node, 0);

                        //printf("third\n"); print_tree();
                        if (node == root) root = nullptr;
                        delete node;
                        has_deleted = 1;
                        return nullptr;
                        //printf("fifth\n"); print_tree();
                    }
                    else //one child of a node
                    {

                        if (node == root) root = node;
                        delete node;
                        has_deleted = 1;
                        tmp = delete_node_phone_EQ_and_group_GE(tmp, cmd);
                        return tmp;
                    }
                    //printf("fourth\n"); print_tree();
                }
                else
                {
                    //printf("first\n"); print_tree();
                    //printf("3\n");

                    //node with two children:

                    tree_node* tmp = find_min(node->right);

                    //copy data
                    node->data = tmp->data;
                    //printf("first + 1 / 2\n"); print_tree();
                    has_deleted = 1;
                    node->right = killer(node->right, tmp->data);
                }

                node = balansirovka(node);
            }
        }

        return balansirovka(node);
    }

    void delete_phone_EQ_and_group_GE(command& cmd)
    {
        delete_node_phone_EQ_and_group_GE(root, cmd);
        while(has_deleted)
        {
            has_deleted = 0;
            delete_node_phone_EQ_and_group_GE(root, cmd);
        }
    }

    //-----conditions in basic form for tree ------------------------------

    void select_cond_NAME_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_NAME_EQ(node->data))
        {
            node->data->print();
        }
        select_cond_NAME_EQ(node->left, cmd);
        select_cond_NAME_EQ(node->right, cmd);
    }

    void select_cond_NAME_EQ(command &cmd) { select_cond_NAME_EQ(root, cmd); }

    void select_cond_NAME_LIKE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_NAME_LIKE(node->data))
        {
            node->data->print();
        }
        select_cond_NAME_LIKE(node->left, cmd);
        select_cond_NAME_LIKE(node->right, cmd);
    }

    void select_cond_NAME_LIKE(command &cmd) { select_cond_NAME_LIKE(root, cmd); }

    //-----

    void select_cond_PHONE_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            node->data->print();
            select_cond_PHONE_EQ(node->left, cmd);
            select_cond_PHONE_EQ(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                 select_cond_PHONE_EQ(node->right, cmd);
                 return;
            }
            //GT
            select_cond_PHONE_EQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ(command &cmd) { select_cond_PHONE_EQ(root, cmd); }

    void select_cond_PHONE_NEQ(tree_node* node, command& cmd)//I DO USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data)) node->data->print();


        select_cond_PHONE_NEQ(node->left, cmd);
        select_cond_PHONE_NEQ(node->right, cmd);
    }
    void select_cond_PHONE_NEQ(command& cmd) { select_cond_PHONE_NEQ(root, cmd); }

    void select_cond_PHONE_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            node->data->print();
            select_cond_PHONE_LT(node->left, cmd);
            select_cond_PHONE_LT(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            node->data->print();
            select_cond_PHONE_LE(node->left, cmd);
            select_cond_PHONE_LE(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE(node->left, cmd);
        }
    }

    void select_cond_PHONE_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            node->data->print();
            select_cond_PHONE_GT(node->left, cmd);
            select_cond_PHONE_GT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            node->data->print();
            select_cond_PHONE_GE(node->left, cmd);
            select_cond_PHONE_GE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE(node->right, cmd);
        }
    }
    void select_cond_PHONE_GE(command& cmd) { select_cond_PHONE_GE(root, cmd); }


    //----------------combinations------------------------

    void select_cond_PHONE_EQ_AND_NAME_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_NAME_EQ(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_NAME_EQ(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_EQ(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_EQ(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_EQ(node->left, cmd);
        }
    }
    void select_cond_PHONE_EQ_AND_NAME_EQ(command& cmd) { select_cond_PHONE_EQ_AND_NAME_EQ(root, cmd); }

    void select_cond_PHONE_EQ_AND_NAME_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_NAME_NEQ(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_NAME_NEQ(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_NEQ(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_NEQ(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_NEQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_NAME_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_NAME_LT(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_NAME_LT(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_LT(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_LT(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_LT(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_NAME_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data))
        {
            if (cmd.cmp_NAME_GT(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_NAME_GT(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_GT(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_GT(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_GT(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_NAME_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_NAME_GE(node->data)) node->data->print(); //cmd <= data
            select_cond_PHONE_EQ_AND_NAME_GE(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_GE(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_GE(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_GE(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_NAME_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_NAME_LE(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_NAME_LE(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_LE(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_LE(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_LE(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_NAME_LIKE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_NAME_LIKE(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_NAME_LIKE(node->left, cmd);
            select_cond_PHONE_EQ_AND_NAME_LIKE(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_NAME_LIKE(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_NAME_LIKE(node->left, cmd);
        }
    }

    //-------  phone NEQ--------

    void select_cond_PHONE_NEQ_AND_NAME_EQ(tree_node* node, command& cmd)//I DO NOT USE IT
    {                                                                                    //IT WORKS BETTER IN LIST
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data) && (cmd.cmp_NAME_EQ(node->data)))
            node->data->print();


        select_cond_PHONE_NEQ_AND_NAME_EQ(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_EQ(node->right, cmd);
    }

    void select_cond_PHONE_NEQ_AND_NAME_NEQ(tree_node* node, command& cmd)//I DO NOT USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data) && (cmd.cmp_NAME_NEQ(node->data)))
            node->data->print();

        select_cond_PHONE_NEQ_AND_NAME_NEQ(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_NEQ(node->right, cmd);
    }

    void select_cond_PHONE_NEQ_AND_NAME_LT(tree_node* node, command& cmd)//I DO NOT USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data) && (cmd.cmp_NAME_LT(node->data)))
            node->data->print();

        select_cond_PHONE_NEQ_AND_NAME_LT(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_LT(node->right, cmd);
    }

    void select_cond_PHONE_NEQ_AND_NAME_GT(tree_node* node, command& cmd)//I DO NOT USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data) && (cmd.cmp_NAME_GT(node->data)))
            node->data->print();

        select_cond_PHONE_NEQ_AND_NAME_GT(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_GT(node->right, cmd);
    }

    void select_cond_PHONE_NEQ_AND_NAME_GE(tree_node* node, command& cmd)//I DO NOT USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data) && (cmd.cmp_NAME_GE(node->data)))
            node->data->print(); //cmd <= data

        select_cond_PHONE_NEQ_AND_NAME_GE(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_GE(node->right, cmd);
    }

    void select_cond_PHONE_NEQ_AND_NAME_LE(tree_node* node, command& cmd)//I DO NOT USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data) && (cmd.cmp_NAME_LE(node->data)))
            node->data->print();

        select_cond_PHONE_NEQ_AND_NAME_LE(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_LE(node->right, cmd);
    }

    void select_cond_PHONE_NEQ_AND_NAME_LIKE(tree_node* node, command& cmd)//I DO NOT USE IT
    {
        if (!node) return;

        if (cmd.cmp_PHONE_NEQ(node->data) && (cmd.cmp_NAME_LIKE(node->data)))
            node->data->print();

        select_cond_PHONE_NEQ_AND_NAME_LIKE(node->left, cmd);
        select_cond_PHONE_NEQ_AND_NAME_LIKE(node->right, cmd);
    }

    //-----------phone_LT + name

    void select_cond_PHONE_LT_AND_NAME_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_EQ(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_EQ(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_EQ(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_EQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_NAME_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_NEQ(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_NEQ(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_NEQ(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_NEQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_NAME_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_LT(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_LT(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_LT(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_LT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_NAME_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_GT(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_GT(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_GT(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_GT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_NAME_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_GE(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_GE(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_GE(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_GE(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_NAME_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_LE(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_LE(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_LE(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_LE(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_NAME_LIKE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_NAME_LIKE(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_NAME_LIKE(node->left, cmd);
            select_cond_PHONE_LT_AND_NAME_LIKE(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_NAME_LIKE(node->left, cmd);
        }
    }

    //---------------phone_GT + name
    //TODO
    void select_cond_PHONE_GT_AND_NAME_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_EQ(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_EQ(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_EQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_EQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_NAME_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_NEQ(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_NEQ(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_NEQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_NEQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_NAME_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_LT(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_LT(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_LT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_LT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_NAME_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_GT(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_GT(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_GT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_GT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_NAME_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_LE(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_LE(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_LE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_LE(node->right, cmd);
        }
    }


    void select_cond_PHONE_GT_AND_NAME_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_GE(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_GE(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_GE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_GE(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_NAME_LIKE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_NAME_LIKE(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_NAME_LIKE(node->left, cmd);
            select_cond_PHONE_GT_AND_NAME_LIKE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_NAME_LIKE(node->right, cmd);
        }
    }

    //-----------phone_GE + name
    //TODO
    void select_cond_PHONE_GE_AND_NAME_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_EQ(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_EQ(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_EQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_EQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_NAME_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_NEQ(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_NEQ(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_NEQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_NEQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_NAME_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_LT(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_LT(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_LT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_LT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_NAME_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_GT(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_GT(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_GT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_GT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_NAME_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_LE(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_LE(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_LE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_LE(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_NAME_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_GE(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_GE(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_GE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_GE(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_NAME_LIKE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_NAME_LIKE(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_NAME_LIKE(node->left, cmd);
            select_cond_PHONE_GE_AND_NAME_LIKE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_NAME_LIKE(node->right, cmd);
        }
    }

    //----------phone_LE + name
    //TODO
    void select_cond_PHONE_LE_AND_NAME_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_NAME_EQ(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_NAME_EQ(node->left, cmd);
            select_cond_PHONE_LE_AND_NAME_EQ(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_NAME_EQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_NAME_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_NAME_NEQ(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_NAME_NEQ(node->left, cmd);
            select_cond_PHONE_LE_AND_NAME_NEQ(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_NAME_NEQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_NAME_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_NAME_LT(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_NAME_LT(node->left, cmd);
            select_cond_PHONE_LE_AND_NAME_LT(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_NAME_LT(node->left, cmd);
        }
    }
    //-----phone + group
    //pgone _EQ+group

    void select_cond_PHONE_EQ_AND_GROUP_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_GROUP_EQ(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_GROUP_EQ(node->left, cmd);
            select_cond_PHONE_EQ_AND_GROUP_EQ(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_GROUP_EQ(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_GROUP_EQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_GROUP_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_GROUP_NEQ(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_GROUP_NEQ(node->left, cmd);
            select_cond_PHONE_EQ_AND_GROUP_NEQ(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_GROUP_NEQ(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_GROUP_NEQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_GROUP_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_GROUP_LT(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_GROUP_LT(node->left, cmd);
            select_cond_PHONE_EQ_AND_GROUP_LT(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_GROUP_LT(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_GROUP_LT(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_GROUP_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data))
        {
            if (cmd.cmp_GROUP_GT(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_GROUP_GT(node->left, cmd);
            select_cond_PHONE_EQ_AND_GROUP_GT(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_GROUP_GT(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_GROUP_GT(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_GROUP_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_GROUP_GE(node->data)) node->data->print(); //cmd <= data
            select_cond_PHONE_EQ_AND_GROUP_GE(node->left, cmd);
            select_cond_PHONE_EQ_AND_GROUP_GE(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_GROUP_GE(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_GROUP_GE(node->left, cmd);
        }
    }

    void select_cond_PHONE_EQ_AND_GROUP_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_EQ(node->data))
        {
            if (cmd.cmp_GROUP_LE(node->data)) node->data->print();
            select_cond_PHONE_EQ_AND_GROUP_LE(node->left, cmd);
            select_cond_PHONE_EQ_AND_GROUP_LE(node->right, cmd);
        }
        else
        {
            if (cmd.cmp_PHONE_LT(node->data)) // cmd. > node->data
            {
                select_cond_PHONE_EQ_AND_GROUP_LE(node->right, cmd);
                return;
            }
            //GT
            select_cond_PHONE_EQ_AND_GROUP_LE(node->left, cmd);
        }
    }

    //--phone_lt+GROUP

    void select_cond_PHONE_LT_AND_GROUP_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_GROUP_EQ(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_GROUP_EQ(node->left, cmd);
            select_cond_PHONE_LT_AND_GROUP_EQ(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_GROUP_EQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_GROUP_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_GROUP_NEQ(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_GROUP_NEQ(node->left, cmd);
            select_cond_PHONE_LT_AND_GROUP_NEQ(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_GROUP_NEQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_GROUP_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_GROUP_LT(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_GROUP_LT(node->left, cmd);
            select_cond_PHONE_LT_AND_GROUP_LT(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_GROUP_LT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_GROUP_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_GROUP_GT(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_GROUP_GT(node->left, cmd);
            select_cond_PHONE_LT_AND_GROUP_GT(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_GROUP_GT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_GROUP_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_GROUP_GE(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_GROUP_GE(node->left, cmd);
            select_cond_PHONE_LT_AND_GROUP_GE(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_GROUP_GE(node->left, cmd);
        }
    }

    void select_cond_PHONE_LT_AND_GROUP_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LT(node->data)) //data < cmd
        {
            if (cmd.cmp_GROUP_LE(node->data)) node->data->print();
            select_cond_PHONE_LT_AND_GROUP_LE(node->left, cmd);
            select_cond_PHONE_LT_AND_GROUP_LE(node->right, cmd);
        }
        else // data >= cmd
        {
            select_cond_PHONE_LT_AND_GROUP_LE(node->left, cmd);
        }
    }


    //---------------phone_GT + GROUP
    //TODO
    void select_cond_PHONE_GT_AND_GROUP_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_GROUP_EQ(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_GROUP_EQ(node->left, cmd);
            select_cond_PHONE_GT_AND_GROUP_EQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_GROUP_EQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_GROUP_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_GROUP_NEQ(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_GROUP_NEQ(node->left, cmd);
            select_cond_PHONE_GT_AND_GROUP_NEQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_GROUP_NEQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_GROUP_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_GROUP_LT(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_GROUP_LT(node->left, cmd);
            select_cond_PHONE_GT_AND_GROUP_LT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_GROUP_LT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_GROUP_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_GROUP_GT(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_GROUP_GT(node->left, cmd);
            select_cond_PHONE_GT_AND_GROUP_GT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_GROUP_GT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GT_AND_GROUP_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_GROUP_LE(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_GROUP_LE(node->left, cmd);
            select_cond_PHONE_GT_AND_GROUP_LE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_GROUP_LE(node->right, cmd);
        }
    }


    void select_cond_PHONE_GT_AND_GROUP_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GT(node->data)) //data > cmd
        {
            if (cmd.cmp_GROUP_GE(node->data)) node->data->print();
            select_cond_PHONE_GT_AND_GROUP_GE(node->left, cmd);
            select_cond_PHONE_GT_AND_GROUP_GE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GT_AND_GROUP_GE(node->right, cmd);
        }
    }


    //-----------phone_GE + GROUP
    //TODO
    void select_cond_PHONE_GE_AND_GROUP_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_GROUP_EQ(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_GROUP_EQ(node->left, cmd);
            select_cond_PHONE_GE_AND_GROUP_EQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_GROUP_EQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_GROUP_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_GROUP_NEQ(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_GROUP_NEQ(node->left, cmd);
            select_cond_PHONE_GE_AND_GROUP_NEQ(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_GROUP_NEQ(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_GROUP_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_GROUP_LT(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_GROUP_LT(node->left, cmd);
            select_cond_PHONE_GE_AND_GROUP_LT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_GROUP_LT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_GROUP_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_GROUP_GT(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_GROUP_GT(node->left, cmd);
            select_cond_PHONE_GE_AND_GROUP_GT(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_GROUP_GT(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_GROUP_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_GROUP_LE(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_GROUP_LE(node->left, cmd);
            select_cond_PHONE_GE_AND_GROUP_LE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_GROUP_LE(node->right, cmd);
        }
    }

    void select_cond_PHONE_GE_AND_GROUP_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_GE(node->data)) //data >= cmd
        {
            if (cmd.cmp_GROUP_GE(node->data)) node->data->print();
            select_cond_PHONE_GE_AND_GROUP_GE(node->left, cmd);
            select_cond_PHONE_GE_AND_GROUP_GE(node->right, cmd);
        }
        else
        {
            select_cond_PHONE_GE_AND_GROUP_GE(node->right, cmd);
        }
    }

    //----------phone_LE + GROUP
    //TODO
    void select_cond_PHONE_LE_AND_GROUP_EQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_GROUP_EQ(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_GROUP_EQ(node->left, cmd);
            select_cond_PHONE_LE_AND_GROUP_EQ(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_GROUP_EQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_GROUP_NEQ(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_GROUP_NEQ(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_GROUP_NEQ(node->left, cmd);
            select_cond_PHONE_LE_AND_GROUP_NEQ(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_GROUP_NEQ(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_GROUP_LT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_GROUP_LT(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_GROUP_LT(node->left, cmd);
            select_cond_PHONE_LE_AND_GROUP_LT(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_GROUP_LT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_GROUP_GT(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_GROUP_GT(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_GROUP_GT(node->left, cmd);
            select_cond_PHONE_LE_AND_GROUP_GT(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_GROUP_GT(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_GROUP_LE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_GROUP_LE(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_GROUP_LE(node->left, cmd);
            select_cond_PHONE_LE_AND_GROUP_LE(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_GROUP_LE(node->left, cmd);
        }
    }

    void select_cond_PHONE_LE_AND_GROUP_GE(tree_node* node, command& cmd)
    {
        if (!node) return;

        if (cmd.cmp_PHONE_LE(node->data)) //data <= cmd
        {
            if (cmd.cmp_GROUP_GE(node->data)) node->data->print();
            select_cond_PHONE_LE_AND_GROUP_GE(node->left, cmd);
            select_cond_PHONE_LE_AND_GROUP_GE(node->right, cmd);
        }
        else //data > cmd
        {
            select_cond_PHONE_LE_AND_GROUP_GE(node->left, cmd);
        }
    }
    //--delete
    //--phone+group




#define MAX_DEPTH 400

    void print_tree(tree_node* node, int depth)
    {
        //curr и root передаются и не используются
        if (depth > MAX_DEPTH) return;


        if (node)
        {
            node->data->print(depth);

            schet++;
            print_tree(node->left, depth + 1);
            print_tree(node->right, depth + 1);
        }
        else return;
    }

    void print_tree()
    {
        schet = 0;
        if (!root) { printf("Empty tree\n\n"); return; }

        print_tree(root, 0);
        printf("chet = %d\n", schet);
        if (schet == 0) root->data->print();
        schet = 0;
    }
    void print() { print_tree(); }
#undef MAX_DEPTH

    void add_right(tree_node* right)
    { curr->set_right(right); }
    void add_left(tree_node* left)
    { curr->set_left(left); }
    void add_root(tree_node* new_root)
    { root = new_root; }

    tree_node* get_left() const
    { return curr->left; }
    tree_node* get_right() const
    { return curr->right; }
    tree_node* get_root() const
    { return root; }

};

#endif //TREE_H


//tree_node delete_node(T& data)
//{ return delete_node(root, data); }

/*tree_node* delete_node_phone_EQ(tree_node* node, list_node* data, int flag = 0) //flag = 1 -> we are not doing li.delete_node()
 {
 //data was not the best choice indeed

 if (!node)
 return nullptr;

 if (data->spec_cmp_phone_lt(*(node->data))) // data < *(node->data)
 node->left = delete_node_phone_EQ(node->left, data, flag);

 else if (data->spec_cmp_phone_gt(*(node->data)))//data > *(node->data)
 node->right = delete_node_phone_EQ(node->right, data, flag);

 //equality => delete
 else
 {
 //node with <=1 child
 if(!node->left || !node->right)
 {
 tree_node *tmp = (node->left) ? (node->left) : (node->right);

 //zero child case
 if (!tmp)
 {
 //printf("second\n"); print_tree(node, 0);
 //printf("node = %s, left = %p, right = %p\n", node->data->get_name(), (void*)node->left, (void*)node->right);
 if (!flag)
 //printf("third\n"); print_tree();
 //printf("0 chaild\n");
 tmp = node;
 node = nullptr;
 //printf("fifth\n"); print_tree();
 }
 else // *node = *tmp
 {
 if (!flag)
 //printf("1 chaild\n");
 node->data = tmp->data;
 node->left = tmp->left;
 node->right = tmp->right;
 node->height = tmp->height;
 }
 //^^^ One child of a node^^^

 //delete tmp both in tree and list

 delete tmp;
 //printf("fourth\n"); print_tree();
 }
 else
 {
 //printf("first\n"); print_tree();
 //printf("3\n");

 //node with two children:
 tree_node* tmp = find_min(node->right);

 //copy data
 node->data = tmp->data;
 //printf("first + 1 / 2\n"); print_tree();

 node->right = delete_node_phone_EQ(node->right, tmp->data, 1);
 }
 }

 if (!node)
 return nullptr;

 //change height
 node->height = 1 + max_2(get_height(node->left), get_height(node->right));

 int balance = get_balance(node);

 //4 cases

 // L L
 if (balance > 1 && get_balance(node->left) >= 0)
 return right_rotate(node);

 // L R
 if (balance > 1 && get_balance(node->left) < 0)
 {
 node->left = left_rotate(node->left);
 return right_rotate(node);
 }

 // R R
 if (balance < -1 && get_balance(node->right) <= 0)
 return left_rotate(node);

 // R L
 if (balance < -1 && get_balance(node->right) > 0)
 {
 node->right = right_rotate(node->right);
 return left_rotate(node);
 }

 return node;
 }*/
