#ifndef DATABASE2_H
#define DATABASE2_H

#include "database1.h"


class database2 //the biggest struct;
{
private:
    list li; //a container
    kilo_tree kilo_tr; //kilo_tree_node is basically db1 (group)
    database1 big_db1; //for phone & name
    //hash parameter is stored in kilo_tree
    //as well as for_delete_group
public:

    database2() = default;
    ~database2() = default;

    void set_p(int value) { kilo_tr.set_p(value); big_db1.set_p(value); }    int get_p() { return kilo_tr.get_p(); }

    void how_good_is_hash() { big_db1.how_good_is_hash(); }


    void print()
    {
        printf("list is fine:\n");
        li.print_list();
        printf("tree is fine:\n");
        kilo_tr.print();
        printf("database1:\n");
        big_db1.print();
    }

    int read (FILE *fp)
    {
        //delete_tree();

        list_node *next;
        int ret;
        next = new list_node();
        if (!next) return -1;
        ret = next->read(fp);
        //printf("ret1 = %d\n", ret);
        if (ret < 0)
        {
            // incorrent read
            delete next;
            return -1;
        }

        if (ret > 0)//empty file
        {
            delete next;
            return 0;
        }

        kilo_tr.add_root(next);
        li.set_head(next);
        big_db1.big_init(next);

        //insert other elements
        for ( ; ; )
        {
            next = new list_node;
            if (!next)
                return 2;
            ret = next->read(fp);
            //printf("ret2 = %d\n", ret);
            switch (ret)
            {
                case 0:
                    break;
                case -1:
                    printf("Cannot read\n");
                    //delete_tree();
                    return -1;
                case 1:
                    delete next; //leak danger
                    return 0;
                case -2:
                    printf("Not enough memory\n");
                    //delete_tree();
                    return 1;
                case 2:
                    printf("Not enough memory\n");
                    //delete_tree();
                    return 2;

                default:
                    printf("bad stuff number %d\n", ret);
                    //delete_tree();
                    return ret;
            }
            //new insertion
            if (kilo_tr.insert(next)) { li.add_before_head(next); big_db1.big_insert(next);}
        } //end of for-cycle insertion

        return 0; //never
    }


    void op_insert(command &cmd)
    {
        list_node *node = new list_node();

        node->init(cmd.get_name(), cmd.phone, cmd.group);

        if (kilo_tr.insert(node)) { li.add_before_head(node); big_db1.big_insert(node); }
        //else delete node;
    }

    void select_group_EQ(command& cmd)
    {
        kilo_tr.select_group_EQ(cmd);
    }

    //void kilo_tree::delete_node_group_EQ_and_phone_EQ(kilo_tree_node* node, command& cmd)
    void delete_group_EQ_and_phone_EQ(command& cmd)
    {
        kilo_tr.delete_node_group_EQ_and_phone_EQ(cmd);
        while (kilo_tr.get_if_action())
        {
            //printf("1\n");
            big_db1.killer(kilo_tr.get_if_action());
            li.delete_node(kilo_tr.get_if_action());
            kilo_tr.delete_node_group_EQ_and_phone_EQ(cmd);
            //printf("2\n");
        }
    }

    void delete_group_EQ_and_name_EQ(command& cmd)
    {
        kilo_tr.delete_node_group_EQ_and_name_EQ(cmd);
        while (kilo_tr.get_if_action())
        {
            //printf("1\n");
            big_db1.killer(kilo_tr.get_if_action());
            li.delete_node(kilo_tr.get_if_action());
            kilo_tr.delete_node_group_EQ_and_name_EQ(cmd);
            //printf("2\n");
        }
    }

    void delete_group_EQ(command& cmd)
    {
        kilo_tr.delete_node_group_EQ(cmd);
        while (kilo_tr.get_if_action())
        {
            //printf("1\n");
            big_db1.killer(kilo_tr.get_if_action());
            li.delete_node(kilo_tr.get_if_action());

            kilo_tr.delete_node_group_EQ(cmd);
            //printf("2\n");
        }
    }



    void delete_phone_EQ_and_name_EQ(command& cmd)
    {
        big_db1.delete_node_phone_EQ_and_name_EQ(cmd);

        while (big_db1.get_if_action())
        {
            //printf("1\n");
            //printf("pointer = %p\n", (void*) node->data->get_if_action());
            kilo_tr.killer(big_db1.get_if_action());
            li.delete_node(big_db1.get_if_action());
            //printf("3\n");
            big_db1.delete_node_phone_EQ_and_name_EQ(cmd);
            //printf("2\n");
        }
    }

    void delete_phone_EQ(command& cmd)
    {
        big_db1.big_delete_node_phone_EQ(cmd);

        //big_db1.get_if_action()->print();

        while (big_db1.get_if_action())
        {
            //printf("1\n");
            //printf("pointer = %p\n", (void*) node->data->get_if_action());
            kilo_tr.killer(big_db1.get_if_action());
            li.delete_node(big_db1.get_if_action());
            //printf("3\n");
            big_db1.big_delete_node_phone_EQ(cmd);
            //printf("2\n");
        }
    }

    void delete_name_EQ(command& cmd)
    {
        big_db1.delete_node_name_EQ(cmd);

        while (big_db1.get_if_action())
        {
            //printf("1\n");
            //printf("pointer = %p\n", (void*) node->data->get_if_action());
            kilo_tr.killer(big_db1.get_if_action());
            li.delete_node(big_db1.get_if_action());
            //printf("3\n");
            big_db1.delete_node_name_EQ(cmd);
            //printf("2\n");
        }
    }



    /*void delete_phone_EQ_and_name_EQ(kilo_tree_node* node, command& cmd)
    {
        if (!node) return;

        node->data->delete_node_phone_EQ_and_name_EQ(cmd);
        while (node->data->get_if_action())
        {
            //printf("1\n");
            //printf("pointer = %p\n", (void*) node->data->get_if_action());
            li.delete_node(node->data->get_if_action());
            //printf("3\n");
            node->data->delete_node_phone_EQ_and_name_EQ(cmd);
            //printf("2\n");
        }

        delete_phone_EQ_and_name_EQ(node->right, cmd);
        delete_phone_EQ_and_name_EQ(node->left, cmd);
    }

    void delete_phone_EQ_and_name_EQ(command& cmd) { delete_phone_EQ_and_name_EQ(kilo_tr.get_root(), cmd); }*/

    /*void delete_phone_EQ(kilo_tree_node* node, command& cmd)
    {
        if (!node) return;

        node->data->delete_node_phone_EQ(cmd);
        while (node->data->get_if_action())
        {
            //printf("1\n");
            //printf("pointer = %p\n", (void*) node->data->get_if_action());
            li.delete_node(node->data->get_if_action());
            //printf("3\n");
            node->data->delete_node_phone_EQ(cmd);
            //printf("2\n");
        }

        delete_phone_EQ(node->right, cmd);
        delete_phone_EQ(node->left, cmd);
    }

    void delete_phone_EQ(command& cmd) { delete_phone_EQ(kilo_tr.get_root(), cmd); }*/


    /*void delete_name_EQ(kilo_tree_node* node, command& cmd)
    {
        if (!node) return;

        node->data->delete_node_name_EQ(cmd);
        while (node->data->get_if_action())
        {
            //printf("1\n");
            //printf("pointer = %p\n", (void*) node->data->get_if_action());
            li.delete_node(node->data->get_if_action());
            //printf("3\n");
            node->data->delete_node_name_EQ(cmd);
            //printf("2\n");
        }

        delete_name_EQ(node->right, cmd);
        delete_name_EQ(node->left, cmd);
    }

    void delete_name_EQ(command& cmd) { delete_name_EQ(kilo_tr.get_root(), cmd); }*/

    enum command_funcs
    {
        ERROR, S_ALL,

        ///-----------------------------------------------------
        S_NAME_EQ,  S_NAME_NEQ,  S_NAME_LT,  S_NAME_GT,  S_NAME_LE,  S_NAME_GE,  S_NAME_LIKE,
        S_PHONE_EQ, S_PHONE_NEQ,  S_PHONE_LT,  S_PHONE_GT,  S_PHONE_LE,  S_PHONE_GE,
        S_GROUP_EQ, S_GROUP_NEQ,  S_GROUP_LT,  S_GROUP_GT,  S_GROUP_LE,  S_GROUP_GE,

        //-----------------------------------------------------------------------------------------------------------------------

        S_PHONE_EQ_OR_NAME_EQ, S_PHONE_EQ_OR_NAME_NEQ, S_PHONE_EQ_OR_NAME_LT, S_PHONE_EQ_OR_NAME_GT, S_PHONE_EQ_OR_NAME_GE, S_PHONE_EQ_OR_NAME_LE, S_PHONE_EQ_OR_NAME_LIKE,
        S_PHONE_EQ_AND_NAME_EQ, S_PHONE_EQ_AND_NAME_NEQ, S_PHONE_EQ_AND_NAME_LT, S_PHONE_EQ_AND_NAME_GT, S_PHONE_EQ_AND_NAME_GE, S_PHONE_EQ_AND_NAME_LE, S_PHONE_EQ_AND_NAME_LIKE,

        S_PHONE_NEQ_OR_NAME_EQ, S_PHONE_NEQ_OR_NAME_NEQ, S_PHONE_NEQ_OR_NAME_LT, S_PHONE_NEQ_OR_NAME_GT, S_PHONE_NEQ_OR_NAME_GE, S_PHONE_NEQ_OR_NAME_LE, S_PHONE_NEQ_OR_NAME_LIKE,
        S_PHONE_NEQ_AND_NAME_EQ, S_PHONE_NEQ_AND_NAME_NEQ, S_PHONE_NEQ_AND_NAME_LT, S_PHONE_NEQ_AND_NAME_GT, S_PHONE_NEQ_AND_NAME_GE, S_PHONE_NEQ_AND_NAME_LE, S_PHONE_NEQ_AND_NAME_LIKE,

        S_PHONE_LT_OR_NAME_EQ, S_PHONE_LT_OR_NAME_NEQ, S_PHONE_LT_OR_NAME_LT, S_PHONE_LT_OR_NAME_GT, S_PHONE_LT_OR_NAME_GE, S_PHONE_LT_OR_NAME_LE, S_PHONE_LT_OR_NAME_LIKE,
        S_PHONE_LT_AND_NAME_EQ, S_PHONE_LT_AND_NAME_NEQ, S_PHONE_LT_AND_NAME_LT, S_PHONE_LT_AND_NAME_GT, S_PHONE_LT_AND_NAME_GE, S_PHONE_LT_AND_NAME_LE, S_PHONE_LT_AND_NAME_LIKE,

        S_PHONE_GT_OR_NAME_EQ, S_PHONE_GT_OR_NAME_NEQ, S_PHONE_GT_OR_NAME_LT, S_PHONE_GT_OR_NAME_GT, S_PHONE_GT_OR_NAME_GE, S_PHONE_GT_OR_NAME_LE, S_PHONE_GT_OR_NAME_LIKE,
        S_PHONE_GT_AND_NAME_EQ, S_PHONE_GT_AND_NAME_NEQ, S_PHONE_GT_AND_NAME_LT, S_PHONE_GT_AND_NAME_GT, S_PHONE_GT_AND_NAME_GE, S_PHONE_GT_AND_NAME_LE, S_PHONE_GT_AND_NAME_LIKE,

        S_PHONE_LE_OR_NAME_EQ, S_PHONE_LE_OR_NAME_NEQ, S_PHONE_LE_OR_NAME_LT, S_PHONE_LE_OR_NAME_GT, S_PHONE_LE_OR_NAME_GE, S_PHONE_LE_OR_NAME_LE, S_PHONE_LE_OR_NAME_LIKE,
        S_PHONE_LE_AND_NAME_EQ, S_PHONE_LE_AND_NAME_NEQ, S_PHONE_LE_AND_NAME_LT, S_PHONE_LE_AND_NAME_GT, S_PHONE_LE_AND_NAME_GE, S_PHONE_LE_AND_NAME_LE, S_PHONE_LE_AND_NAME_LIKE,

        S_PHONE_GE_OR_NAME_EQ, S_PHONE_GE_OR_NAME_NEQ, S_PHONE_GE_OR_NAME_LT, S_PHONE_GE_OR_NAME_GT, S_PHONE_GE_OR_NAME_GE, S_PHONE_GE_OR_NAME_LE, S_PHONE_GE_OR_NAME_LIKE,
        S_PHONE_GE_AND_NAME_EQ, S_PHONE_GE_AND_NAME_NEQ, S_PHONE_GE_AND_NAME_LT, S_PHONE_GE_AND_NAME_GT, S_PHONE_GE_AND_NAME_GE, S_PHONE_GE_AND_NAME_LE, S_PHONE_GE_AND_NAME_LIKE,

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------

        S_GROUP_EQ_OR_NAME_EQ, S_GROUP_EQ_OR_NAME_NEQ, S_GROUP_EQ_OR_NAME_LT, S_GROUP_EQ_OR_NAME_GT, S_GROUP_EQ_OR_NAME_GE, S_GROUP_EQ_OR_NAME_LE, S_GROUP_EQ_OR_NAME_LIKE,
        S_GROUP_EQ_AND_NAME_EQ, S_GROUP_EQ_AND_NAME_NEQ, S_GROUP_EQ_AND_NAME_LT, S_GROUP_EQ_AND_NAME_GT, S_GROUP_EQ_AND_NAME_GE, S_GROUP_EQ_AND_NAME_LE, S_GROUP_EQ_AND_NAME_LIKE,

        S_GROUP_NEQ_OR_NAME_EQ, S_GROUP_NEQ_OR_NAME_NEQ, S_GROUP_NEQ_OR_NAME_LT, S_GROUP_NEQ_OR_NAME_GT, S_GROUP_NEQ_OR_NAME_GE, S_GROUP_NEQ_OR_NAME_LE, S_GROUP_NEQ_OR_NAME_LIKE,
        S_GROUP_NEQ_AND_NAME_EQ, S_GROUP_NEQ_AND_NAME_NEQ, S_GROUP_NEQ_AND_NAME_LT, S_GROUP_NEQ_AND_NAME_GT, S_GROUP_NEQ_AND_NAME_GE, S_GROUP_NEQ_AND_NAME_LE, S_GROUP_NEQ_AND_NAME_LIKE,

        S_GROUP_LT_OR_NAME_EQ, S_GROUP_LT_OR_NAME_NEQ, S_GROUP_LT_OR_NAME_LT, S_GROUP_LT_OR_NAME_GT, S_GROUP_LT_OR_NAME_GE, S_GROUP_LT_OR_NAME_LE, S_GROUP_LT_OR_NAME_LIKE,
        S_GROUP_LT_AND_NAME_EQ, S_GROUP_LT_AND_NAME_NEQ, S_GROUP_LT_AND_NAME_LT, S_GROUP_LT_AND_NAME_GT, S_GROUP_LT_AND_NAME_GE, S_GROUP_LT_AND_NAME_LE, S_GROUP_LT_AND_NAME_LIKE,

        S_GROUP_GT_OR_NAME_EQ, S_GROUP_GT_OR_NAME_NEQ, S_GROUP_GT_OR_NAME_LT, S_GROUP_GT_OR_NAME_GT, S_GROUP_GT_OR_NAME_GE, S_GROUP_GT_OR_NAME_LE, S_GROUP_GT_OR_NAME_LIKE,
        S_GROUP_GT_AND_NAME_EQ, S_GROUP_GT_AND_NAME_NEQ, S_GROUP_GT_AND_NAME_LT, S_GROUP_GT_AND_NAME_GT, S_GROUP_GT_AND_NAME_GE, S_GROUP_GT_AND_NAME_LE, S_GROUP_GT_AND_NAME_LIKE,

        S_GROUP_LE_OR_NAME_EQ, S_GROUP_LE_OR_NAME_NEQ, S_GROUP_LE_OR_NAME_LT, S_GROUP_LE_OR_NAME_GT, S_GROUP_LE_OR_NAME_GE, S_GROUP_LE_OR_NAME_LE, S_GROUP_LE_OR_NAME_LIKE,
        S_GROUP_LE_AND_NAME_EQ, S_GROUP_LE_AND_NAME_NEQ, S_GROUP_LE_AND_NAME_LT, S_GROUP_LE_AND_NAME_GT, S_GROUP_LE_AND_NAME_GE, S_GROUP_LE_AND_NAME_LE, S_GROUP_LE_AND_NAME_LIKE,

        S_GROUP_GE_OR_NAME_EQ, S_GROUP_GE_OR_NAME_NEQ, S_GROUP_GE_OR_NAME_LT, S_GROUP_GE_OR_NAME_GT, S_GROUP_GE_OR_NAME_GE, S_GROUP_GE_OR_NAME_LE, S_GROUP_GE_OR_NAME_LIKE,
        S_GROUP_GE_AND_NAME_EQ, S_GROUP_GE_AND_NAME_NEQ, S_GROUP_GE_AND_NAME_LT, S_GROUP_GE_AND_NAME_GT, S_GROUP_GE_AND_NAME_GE, S_GROUP_GE_AND_NAME_LE, S_GROUP_GE_AND_NAME_LIKE,

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        S_GROUP_EQ_OR_PHONE_EQ, S_GROUP_EQ_OR_PHONE_NEQ, S_GROUP_EQ_OR_PHONE_LT, S_GROUP_EQ_OR_PHONE_GT, S_GROUP_EQ_OR_PHONE_GE, S_GROUP_EQ_OR_PHONE_LE,
        S_GROUP_EQ_AND_PHONE_EQ, S_GROUP_EQ_AND_PHONE_NEQ, S_GROUP_EQ_AND_PHONE_LT, S_GROUP_EQ_AND_PHONE_GT, S_GROUP_EQ_AND_PHONE_GE, S_GROUP_EQ_AND_PHONE_LE,

        S_GROUP_NEQ_OR_PHONE_EQ, S_GROUP_NEQ_OR_PHONE_NEQ, S_GROUP_NEQ_OR_PHONE_LT, S_GROUP_NEQ_OR_PHONE_GT, S_GROUP_NEQ_OR_PHONE_GE, S_GROUP_NEQ_OR_PHONE_LE,
        S_GROUP_NEQ_AND_PHONE_EQ, S_GROUP_NEQ_AND_PHONE_NEQ, S_GROUP_NEQ_AND_PHONE_LT, S_GROUP_NEQ_AND_PHONE_GT, S_GROUP_NEQ_AND_PHONE_GE, S_GROUP_NEQ_AND_PHONE_LE,

        S_GROUP_LT_OR_PHONE_EQ, S_GROUP_LT_OR_PHONE_NEQ, S_GROUP_LT_OR_PHONE_LT, S_GROUP_LT_OR_PHONE_GT, S_GROUP_LT_OR_PHONE_GE, S_GROUP_LT_OR_PHONE_LE,
        S_GROUP_LT_AND_PHONE_EQ, S_GROUP_LT_AND_PHONE_NEQ, S_GROUP_LT_AND_PHONE_LT, S_GROUP_LT_AND_PHONE_GT, S_GROUP_LT_AND_PHONE_GE, S_GROUP_LT_AND_PHONE_LE,

        S_GROUP_GT_OR_PHONE_EQ, S_GROUP_GT_OR_PHONE_NEQ, S_GROUP_GT_OR_PHONE_LT, S_GROUP_GT_OR_PHONE_GT, S_GROUP_GT_OR_PHONE_GE, S_GROUP_GT_OR_PHONE_LE,
        S_GROUP_GT_AND_PHONE_EQ, S_GROUP_GT_AND_PHONE_NEQ, S_GROUP_GT_AND_PHONE_LT, S_GROUP_GT_AND_PHONE_GT, S_GROUP_GT_AND_PHONE_GE, S_GROUP_GT_AND_PHONE_LE,

        S_GROUP_LE_OR_PHONE_EQ, S_GROUP_LE_OR_PHONE_NEQ, S_GROUP_LE_OR_PHONE_LT, S_GROUP_LE_OR_PHONE_GT, S_GROUP_LE_OR_PHONE_GE, S_GROUP_LE_OR_PHONE_LE,
        S_GROUP_LE_AND_PHONE_EQ, S_GROUP_LE_AND_PHONE_NEQ, S_GROUP_LE_AND_PHONE_LT, S_GROUP_LE_AND_PHONE_GT, S_GROUP_LE_AND_PHONE_GE, S_GROUP_LE_AND_PHONE_LE,

        S_GROUP_GE_OR_PHONE_EQ, S_GROUP_GE_OR_PHONE_NEQ, S_GROUP_GE_OR_PHONE_LT, S_GROUP_GE_OR_PHONE_GT, S_GROUP_GE_OR_PHONE_GE, S_GROUP_GE_OR_PHONE_LE,
        S_GROUP_GE_AND_PHONE_EQ, S_GROUP_GE_AND_PHONE_NEQ, S_GROUP_GE_AND_PHONE_LT, S_GROUP_GE_AND_PHONE_GT, S_GROUP_GE_AND_PHONE_GE, S_GROUP_GE_AND_PHONE_LE,


    };

    void op_select(command &cmd)
    {
        command_funcs res;
        li.current = li.head;
        res = S_ALL;

        switch (cmd.c_group)
        {
            case COND_NONE:
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_ALL;
                        break;
                    case EQ:
                        res = S_NAME_EQ;
                        break;
                    case NEQ:
                        res = S_NAME_NEQ;
                        break;
                    case LT:
                        res = S_NAME_LT;
                        break;
                    case GT:
                        res = S_NAME_GT;
                        break;
                    case LE:
                        res = S_NAME_LE;
                        break;
                    case GE:
                        res = S_NAME_GE;
                        break;
                    case LIKE:
                        res = S_NAME_LIKE;
                        break;
                }
                    break;
                    //c_phone=
                case EQ:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_PHONE_EQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 1\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 2\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 3\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 4\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 5\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 6\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_EQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_PHONE_EQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed\n 7");
                            break;
                    }
                        break;
                }
                    break;


                    //c_phone=
                case NEQ:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_PHONE_NEQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 8\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 9\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 10\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 11\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 12\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 13\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_NEQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_PHONE_NEQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 14\n");
                            break;
                    }
                        break;
                }
                    break;







                    //c_phone=
                case LT:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_PHONE_LT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 15\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 16\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 17\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 18\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 19\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 20\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_PHONE_LT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 21\n");
                            break;
                    }
                        break;
                }
                    break;




                    //c_phone=
                case GT:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_PHONE_GT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 22\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 23\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 24\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 25\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 26\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 27\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_PHONE_GT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 28\n");
                            break;
                    }
                        break;
                }
                    break;





                    //c_phone=
                case LE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_PHONE_LE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 29\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 30\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 31\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 32\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 33\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 34\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_LE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_PHONE_LE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 35\n");
                            break;
                    }
                        break;
                }
                    break;





                    //c_phone=
                case GE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_PHONE_GE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 36\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 37\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 38\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 39\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 40\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 41\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_PHONE_GE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_PHONE_GE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("none cond while it needed 42\n");
                            break;
                    }
                        break;
                }
                    break;





                    //c_phone=
                case LIKE:
                    printf("Like with integers 1\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = COND_NONE

            case EQ:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_GROUP_EQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 43");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 44");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 45");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 46");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 47");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 48");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_EQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_GROUP_EQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 49");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is EQ
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_EQ_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = S_GROUP_EQ_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 50");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_EQ_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = S_GROUP_EQ_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 51");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_EQ_OR_PHONE_LT;
                        break;
                    case AND:
                        res = S_GROUP_EQ_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 52");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_EQ_OR_PHONE_GT;
                        break;
                    case AND:
                        res = S_GROUP_EQ_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 53");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_EQ_OR_PHONE_LE;
                        break;
                    case AND:
                        res = S_GROUP_EQ_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 54");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_EQ_OR_PHONE_GE;
                        break;
                    case AND:
                        res = S_GROUP_EQ_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 55");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 2\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = EQ


            case NEQ:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_GROUP_NEQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 56");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 57");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 58");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 59");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 60");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 61");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_NEQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_GROUP_NEQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 62");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is NEQ
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_NEQ_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = S_GROUP_NEQ_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 63");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_NEQ_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = S_GROUP_NEQ_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 64");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_NEQ_OR_PHONE_LT;
                        break;
                    case AND:
                        res = S_GROUP_NEQ_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 65");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_NEQ_OR_PHONE_GT;
                        break;
                    case AND:
                        res = S_GROUP_NEQ_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 66");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_NEQ_OR_PHONE_LE;
                        break;
                    case AND:
                        res = S_GROUP_NEQ_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 67");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_NEQ_OR_PHONE_GE;
                        break;
                    case AND:
                        res = S_GROUP_NEQ_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 68");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 3\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = NEQ




            case LT:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_GROUP_LT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 69");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 70");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 71");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 72");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 73");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 74");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_GROUP_LT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 75");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is LT
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 76");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 77");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_LT;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 78");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_GT;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 79");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_LE;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 80");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_GE;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 81");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 5\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = LT


            case GT:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_GROUP_GT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 82");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 83");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 84");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 85");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 86");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 87");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_GROUP_GT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 88");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is GT
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GT_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = S_GROUP_GT_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 89");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GT_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = S_GROUP_GT_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 90");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GT_OR_PHONE_LT;
                        break;
                    case AND:
                        res = S_GROUP_GT_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 91");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LT_OR_PHONE_GT;
                        break;
                    case AND:
                        res = S_GROUP_LT_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 92");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GT_OR_PHONE_LE;
                        break;
                    case AND:
                        res = S_GROUP_GT_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 93");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GT_OR_PHONE_GE;
                        break;
                    case AND:
                        res = S_GROUP_GT_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 94");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 4\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = GT


            case LE:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_GROUP_LE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 95");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 96");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 97");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 98");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 99");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 100");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_LE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_GROUP_LE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 101");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is LE
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LE_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = S_GROUP_LE_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 102");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LE_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = S_GROUP_LE_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 103");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LE_OR_PHONE_LT;
                        break;
                    case AND:
                        res = S_GROUP_LE_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 104");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LE_OR_PHONE_GT;
                        break;
                    case AND:
                        res = S_GROUP_LE_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 105");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LE_OR_PHONE_LE;
                        break;
                    case AND:
                        res = S_GROUP_LE_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 106");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_LE_OR_PHONE_GE;
                        break;
                    case AND:
                        res = S_GROUP_LE_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 107");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 6\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = LE



            case GE:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = S_GROUP_GE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 108");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 109");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_LT;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 110");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_GT;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 111");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_LE;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 112");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_GE;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 113");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = S_GROUP_GE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = S_GROUP_GE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = ERROR;
                            printf("None cond where it needed 114");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is GE
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GE_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = S_GROUP_GE_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 115");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GE_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = S_GROUP_GE_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 116");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GE_OR_PHONE_LT;
                        break;
                    case AND:
                        res = S_GROUP_GE_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 117");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GE_OR_PHONE_GT;
                        break;
                    case AND:
                        res = S_GROUP_GE_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 118");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GE_OR_PHONE_LE;
                        break;
                    case AND:
                        res = S_GROUP_GE_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 119");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = S_GROUP_GE_OR_PHONE_GE;
                        break;
                    case AND:
                        res = S_GROUP_GE_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = ERROR;
                        printf("None cond where it needed 120");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 7\n");
                    res = ERROR;
                    break;
            }
                break; //end c_group = GE

            case LIKE:
                printf("TERRIBLE ERROR, \"like\" with integers\n");
                break;
        } //end switch (cmd.group)

        if (!li.head) { printf("Selection on an empty set\n"); return; }


        li.current = li.head;
        //the command is stored is res now. What shall we do?
        switch (res) //SURE
        {
                //case ERROR: is below, it does not happen often
            case S_ALL:
                while(li.current)
                { li.current->print(); li.go_next(); } //print all
                break;

                ///-----------------------------------------------------
            case S_NAME_EQ:
                big_db1.select_cond_NAME_EQ(cmd);
                break;

            case S_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;

            case S_NAME_LT:
                while(li.current)
                { if (cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;

            case S_NAME_GT:
                while(li.current)
                { if (cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;

            case S_NAME_LE:
                while(li.current)
                { if (cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_NAME_GE:
                while(li.current)
                { if (cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_NAME_LIKE:
                kilo_tr.select_name_LIKE(cmd);
                break;

                //--phone only

            case S_PHONE_EQ:
                big_db1.get_tree().select_cond_PHONE_EQ(cmd);
                break;
            case S_PHONE_NEQ:
                //tr.select_cond_PHONE_NEQ(tr.root, cmd, *this);
                break;
            case S_PHONE_LT:
                //tr.select_cond_PHONE_LT(tr.root, cmd, *this);
                break;
            case S_PHONE_GT:
                //tr.select_cond_PHONE_GT(tr.root, cmd, *this);
                break;
            case S_PHONE_LE:
                //tr.select_cond_PHONE_LE(tr.root, cmd, *this);
                break;
            case S_PHONE_GE:
                //tr.select_cond_PHONE_GE(tr.root, cmd, *this);
                break;

                //--group only

            case S_GROUP_EQ:
                select_group_EQ(cmd);
                break;
            case S_GROUP_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT:
                kilo_tr.select_group_LT(cmd);
                break;
            case S_GROUP_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current)) li.current->print(); li.go_next();}
                break;


                // PHONE + NAME
                //------------------------phone_eq + name-----------------------------------------------------------------------------------------------

            case S_PHONE_EQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_EQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_EQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_EQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_EQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_EQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_EQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_PHONE_EQ_AND_NAME_EQ:
                kilo_tr.select_phone_EQ_and_name_EQ(cmd);
                break;
            case S_PHONE_EQ_AND_NAME_NEQ:
                //tr.select_cond_PHONE_EQ_AND_NAME_NEQ(tr.root, cmd, *this);
                break;
            case S_PHONE_EQ_AND_NAME_LT:
                //tr.select_cond_PHONE_EQ_AND_NAME_LT(tr.root, cmd, *this);
                break;
            case S_PHONE_EQ_AND_NAME_GT:
                //tr.select_cond_PHONE_EQ_AND_NAME_GT(tr.root, cmd, *this);
                break;
            case S_PHONE_EQ_AND_NAME_GE:
                //tr.select_cond_PHONE_EQ_AND_NAME_GE(tr.root, cmd, *this);
                break;
            case S_PHONE_EQ_AND_NAME_LE:
                //tr.select_cond_PHONE_EQ_AND_NAME_LE(tr.root, cmd, *this);
                break;
            case S_PHONE_EQ_AND_NAME_LIKE:
                //tr.select_cond_PHONE_EQ_AND_NAME_LIKE(tr.root, cmd, *this);
                break;

                //----------------- phone_neq + name --------------------------

            case S_PHONE_NEQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_PHONE_NEQ_AND_NAME_EQ:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_AND_NAME_NEQ:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_AND_NAME_LT:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_AND_NAME_GT:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_AND_NAME_GE:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_AND_NAME_LE:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_NEQ_AND_NAME_LIKE:
                while(li.current) { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


                //-------------------- phone_lt + name -----------------------------


            case S_PHONE_LT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_PHONE_LT_AND_NAME_EQ:
                //tr.select_cond_PHONE_LT_AND_NAME_EQ(tr.root, cmd, *this);
                break;
            case S_PHONE_LT_AND_NAME_NEQ:
                //tr.select_cond_PHONE_LT_AND_NAME_NEQ(tr.root, cmd, *this);
                break;
            case S_PHONE_LT_AND_NAME_LT:
                //tr.select_cond_PHONE_LT_AND_NAME_LT(tr.root, cmd, *this);
                break;
            case S_PHONE_LT_AND_NAME_GT:
                //tr.select_cond_PHONE_LT_AND_NAME_GT(tr.root, cmd, *this);
                break;
            case S_PHONE_LT_AND_NAME_GE:
                //tr.select_cond_PHONE_LT_AND_NAME_GE(tr.root, cmd, *this);
                break;
            case S_PHONE_LT_AND_NAME_LE:
                //tr.select_cond_PHONE_LT_AND_NAME_LE(tr.root, cmd, *this);
                break;
            case S_PHONE_LT_AND_NAME_LIKE:
                //tr.select_cond_PHONE_LT_AND_NAME_LIKE(tr.root, cmd, *this);
                break;


                //---------- PHONE_GT + NAME ----------------------------------------------



            case S_PHONE_GT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_PHONE_GT_AND_NAME_EQ:
                //tr.select_cond_PHONE_GT_AND_NAME_EQ(tr.root, cmd, *this);
                break;
            case S_PHONE_GT_AND_NAME_NEQ:
                //tr.select_cond_PHONE_GT_AND_NAME_NEQ(tr.root, cmd, *this);
                break;
            case S_PHONE_GT_AND_NAME_LT:
                //tr.select_cond_PHONE_GT_AND_NAME_LT(tr.root, cmd, *this);
                break;
            case S_PHONE_GT_AND_NAME_GT:
                //tr.select_cond_PHONE_GT_AND_NAME_GT(tr.root, cmd, *this);
                break;
            case S_PHONE_GT_AND_NAME_GE:
                //tr.select_cond_PHONE_GT_AND_NAME_GE(tr.root, cmd, *this);
                break;
            case S_PHONE_GT_AND_NAME_LE:
                //tr.select_cond_PHONE_GT_AND_NAME_EQ(tr.root, cmd, *this);
                break;
            case S_PHONE_GT_AND_NAME_LIKE:
                //tr.select_cond_PHONE_GT_AND_NAME_LIKE(tr.root, cmd, *this);
                break;




                //---------------PHONE_GE + NAME

            case S_PHONE_GE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_GE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_PHONE_GE_AND_NAME_EQ:
                //tr.select_cond_PHONE_GE_AND_NAME_EQ(tr.root, cmd, *this);
                break;
            case S_PHONE_GE_AND_NAME_NEQ:
                //tr.select_cond_PHONE_GE_AND_NAME_NEQ(tr.root, cmd, *this);
                break;
            case S_PHONE_GE_AND_NAME_LT:
                //tr.select_cond_PHONE_GE_AND_NAME_LT(tr.root, cmd, *this);
                break;
            case S_PHONE_GE_AND_NAME_GT:
                //tr.select_cond_PHONE_GE_AND_NAME_GT(tr.root, cmd, *this);
                break;
            case S_PHONE_GE_AND_NAME_GE:
                //tr.select_cond_PHONE_GE_AND_NAME_GE(tr.root, cmd, *this);
                break;
            case S_PHONE_GE_AND_NAME_LE:
                //tr.select_cond_PHONE_GE_AND_NAME_LE(tr.root, cmd, *this);
                break;
            case S_PHONE_GE_AND_NAME_LIKE:
                //tr.select_cond_PHONE_GE_AND_NAME_LIKE(tr.root, cmd, *this);
                break;

                //------------------------PHONE_LE + MANE



            case S_PHONE_LE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_PHONE_LE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_PHONE_LE_AND_NAME_EQ:
                //tr.select_cond_PHONE_GE_AND_NAME_EQ(tr.root, cmd, *this);
                break;
            case S_PHONE_LE_AND_NAME_NEQ:
                //tr.select_cond_PHONE_GE_AND_NAME_NEQ(tr.root, cmd, *this);
                break;
            case S_PHONE_LE_AND_NAME_LT:
                //tr.select_cond_PHONE_GE_AND_NAME_LT(tr.root, cmd, *this);
                break;
            case S_PHONE_LE_AND_NAME_GT:
                //tr.select_cond_PHONE_GE_AND_NAME_GT(tr.root, cmd, *this);
                break;
            case S_PHONE_LE_AND_NAME_GE:
                //tr.select_cond_PHONE_GE_AND_NAME_GE(tr.root, cmd, *this);
                break;
            case S_PHONE_LE_AND_NAME_LE:
                //tr.select_cond_PHONE_GE_AND_NAME_LE(tr.root, cmd, *this);
                break;
            case S_PHONE_LE_AND_NAME_LIKE:
                //tr.select_cond_PHONE_GE_AND_NAME_LIKE(tr.root, cmd, *this);
                break;


                //-------------------------------------GROUP + NAME-----------------------------------------------------------------------------------------------------------------------------

                //------------------------phone_eq + name-----------------------------------------------------------------------------------------------

            case S_GROUP_EQ_OR_NAME_EQ:
                kilo_tr.select_group_EQ_and_name_EQ(cmd);
                break;
            case S_GROUP_EQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_EQ_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;

                //----------------- phone_neq + name --------------------------

            case S_GROUP_NEQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_NEQ_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


                //-------------------- phone_lt + name -----------------------------


            case S_GROUP_LT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_LT_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


                //---------- GROUP_GT + NAME ----------------------------------------------



            case S_GROUP_GT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_GT_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;




                //---------------GROUP_GE + NAME

            case S_GROUP_GE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_GE_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;

                //------------------------GROUP_LE + MANE



            case S_GROUP_LE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_LE_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_LE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.current->print(); li.go_next();}
                break;





                //---------------------GROUP + PHONE-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------







                //------------------------GROUP_eq + PHONE-----------------------------------------------------------------------------------------------

            case S_GROUP_EQ_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_EQ_AND_PHONE_EQ:
                kilo_tr.select_group_EQ_and_phone_EQ(cmd);
                break;
            case S_GROUP_EQ_AND_PHONE_NEQ:
                kilo_tr.select_group_EQ_and_phone_NEQ (cmd);
                break;
            case S_GROUP_EQ_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_EQ_AND_PHONE_GE:
                kilo_tr.select_group_EQ_and_phone_GE(cmd);
                break;
            case S_GROUP_EQ_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
                //----------------- GROUP_neq + PHONE --------------------------

            case S_GROUP_NEQ_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_NEQ_AND_PHONE_EQ:
                //tr.select_cond_PHONE_EQ_AND_GROUP_NEQ(tr.root, cmd, *this);
                break;
            case S_GROUP_NEQ_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_NEQ_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


                //-------------------- GROUP_lt + PHONE -----------------------------


            case S_GROUP_LT_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;



            case S_GROUP_LT_AND_PHONE_EQ:
                //tr.select_cond_PHONE_EQ_AND_GROUP_LT(tr.root, cmd, *this);
                break;
            case S_GROUP_LT_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LT_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


                //---------- GROUP_GT + PHONE ----------------------------------------------



            case S_GROUP_GT_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_GT_AND_PHONE_EQ:
                //tr.select_cond_PHONE_EQ_AND_GROUP_GT(tr.root, cmd, *this);
                break;
            case S_GROUP_GT_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GT_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;



                //---------------GROUP_GE + PHONE

            case S_GROUP_GE_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_GE_AND_PHONE_EQ:
                //tr.select_cond_PHONE_EQ_AND_GROUP_GE(tr.root, cmd, *this);
                break;
            case S_GROUP_GE_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_GE_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;

                //------------------------GROUP_LE + PHONE



            case S_GROUP_LE_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_LT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_GT(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_GE(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_LE(li.current)) li.current->print(); li.go_next();}
                break;


            case S_GROUP_LE_AND_PHONE_EQ:
                //tr.select_cond_PHONE_EQ_AND_GROUP_LE(tr.root, cmd, *this);
                break;
            case S_GROUP_LE_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.current->print(); li.go_next();}
                break;
            case S_GROUP_LE_AND_PHONE_LT:
                //tr.select_cond_PHONE_LT_AND_GROUP_LE(tr.root, cmd, *this);
                break;
            case S_GROUP_LE_AND_PHONE_GT:
                //tr.select_cond_PHONE_GT_AND_GROUP_LE(tr.root, cmd, *this);
                break;
            case S_GROUP_LE_AND_PHONE_GE:
                //tr.select_cond_PHONE_GE_AND_GROUP_LE(tr.root, cmd, *this);
                break;
            case S_GROUP_LE_AND_PHONE_LE:
                //tr.select_cond_PHONE_LE_AND_GROUP_LE(tr.root, cmd, *this);
                break;


            case ERROR:
                printf("Error. OMG\n");
        } //end switch(res)

        printf("end of selection\n\n");
    }















































    //----||||------------      ||||
    //vvvv here goes delete vvvv











    enum d_command_funcs
    {
        D_ERROR, D_ALL,

        ///-----------------------------------------------------
        D_NAME_EQ,  D_NAME_NEQ,  D_NAME_LT,  D_NAME_GT,  D_NAME_LE,  D_NAME_GE,  D_NAME_LIKE,
        D_PHONE_EQ, D_PHONE_NEQ,  D_PHONE_LT,  D_PHONE_GT,  D_PHONE_LE,  D_PHONE_GE,
        D_GROUP_EQ, D_GROUP_NEQ,  D_GROUP_LT,  D_GROUP_GT,  D_GROUP_LE,  D_GROUP_GE,

        //-----------------------------------------------------------------------------------------------------------------------

        D_PHONE_EQ_OR_NAME_EQ, D_PHONE_EQ_OR_NAME_NEQ, D_PHONE_EQ_OR_NAME_LT, D_PHONE_EQ_OR_NAME_GT, D_PHONE_EQ_OR_NAME_GE, D_PHONE_EQ_OR_NAME_LE, D_PHONE_EQ_OR_NAME_LIKE,
        D_PHONE_EQ_AND_NAME_EQ, D_PHONE_EQ_AND_NAME_NEQ, D_PHONE_EQ_AND_NAME_LT, D_PHONE_EQ_AND_NAME_GT, D_PHONE_EQ_AND_NAME_GE, D_PHONE_EQ_AND_NAME_LE, D_PHONE_EQ_AND_NAME_LIKE,

        D_PHONE_NEQ_OR_NAME_EQ, D_PHONE_NEQ_OR_NAME_NEQ, D_PHONE_NEQ_OR_NAME_LT, D_PHONE_NEQ_OR_NAME_GT, D_PHONE_NEQ_OR_NAME_GE, D_PHONE_NEQ_OR_NAME_LE, D_PHONE_NEQ_OR_NAME_LIKE,
        D_PHONE_NEQ_AND_NAME_EQ, D_PHONE_NEQ_AND_NAME_NEQ, D_PHONE_NEQ_AND_NAME_LT, D_PHONE_NEQ_AND_NAME_GT, D_PHONE_NEQ_AND_NAME_GE, D_PHONE_NEQ_AND_NAME_LE, D_PHONE_NEQ_AND_NAME_LIKE,

        D_PHONE_LT_OR_NAME_EQ, D_PHONE_LT_OR_NAME_NEQ, D_PHONE_LT_OR_NAME_LT, D_PHONE_LT_OR_NAME_GT, D_PHONE_LT_OR_NAME_GE, D_PHONE_LT_OR_NAME_LE, D_PHONE_LT_OR_NAME_LIKE,
        D_PHONE_LT_AND_NAME_EQ, D_PHONE_LT_AND_NAME_NEQ, D_PHONE_LT_AND_NAME_LT, D_PHONE_LT_AND_NAME_GT, D_PHONE_LT_AND_NAME_GE, D_PHONE_LT_AND_NAME_LE, D_PHONE_LT_AND_NAME_LIKE,

        D_PHONE_GT_OR_NAME_EQ, D_PHONE_GT_OR_NAME_NEQ, D_PHONE_GT_OR_NAME_LT, D_PHONE_GT_OR_NAME_GT, D_PHONE_GT_OR_NAME_GE, D_PHONE_GT_OR_NAME_LE, D_PHONE_GT_OR_NAME_LIKE,
        D_PHONE_GT_AND_NAME_EQ, D_PHONE_GT_AND_NAME_NEQ, D_PHONE_GT_AND_NAME_LT, D_PHONE_GT_AND_NAME_GT, D_PHONE_GT_AND_NAME_GE, D_PHONE_GT_AND_NAME_LE, D_PHONE_GT_AND_NAME_LIKE,

        D_PHONE_LE_OR_NAME_EQ, D_PHONE_LE_OR_NAME_NEQ, D_PHONE_LE_OR_NAME_LT, D_PHONE_LE_OR_NAME_GT, D_PHONE_LE_OR_NAME_GE, D_PHONE_LE_OR_NAME_LE, D_PHONE_LE_OR_NAME_LIKE,
        D_PHONE_LE_AND_NAME_EQ, D_PHONE_LE_AND_NAME_NEQ, D_PHONE_LE_AND_NAME_LT, D_PHONE_LE_AND_NAME_GT, D_PHONE_LE_AND_NAME_GE, D_PHONE_LE_AND_NAME_LE, D_PHONE_LE_AND_NAME_LIKE,

        D_PHONE_GE_OR_NAME_EQ, D_PHONE_GE_OR_NAME_NEQ, D_PHONE_GE_OR_NAME_LT, D_PHONE_GE_OR_NAME_GT, D_PHONE_GE_OR_NAME_GE, D_PHONE_GE_OR_NAME_LE, D_PHONE_GE_OR_NAME_LIKE,
        D_PHONE_GE_AND_NAME_EQ, D_PHONE_GE_AND_NAME_NEQ, D_PHONE_GE_AND_NAME_LT, D_PHONE_GE_AND_NAME_GT, D_PHONE_GE_AND_NAME_GE, D_PHONE_GE_AND_NAME_LE, D_PHONE_GE_AND_NAME_LIKE,

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------

        D_GROUP_EQ_OR_NAME_EQ, D_GROUP_EQ_OR_NAME_NEQ, D_GROUP_EQ_OR_NAME_LT, D_GROUP_EQ_OR_NAME_GT, D_GROUP_EQ_OR_NAME_GE, D_GROUP_EQ_OR_NAME_LE, D_GROUP_EQ_OR_NAME_LIKE,
        D_GROUP_EQ_AND_NAME_EQ, D_GROUP_EQ_AND_NAME_NEQ, D_GROUP_EQ_AND_NAME_LT, D_GROUP_EQ_AND_NAME_GT, D_GROUP_EQ_AND_NAME_GE, D_GROUP_EQ_AND_NAME_LE, D_GROUP_EQ_AND_NAME_LIKE,

        D_GROUP_NEQ_OR_NAME_EQ, D_GROUP_NEQ_OR_NAME_NEQ, D_GROUP_NEQ_OR_NAME_LT, D_GROUP_NEQ_OR_NAME_GT, D_GROUP_NEQ_OR_NAME_GE, D_GROUP_NEQ_OR_NAME_LE, D_GROUP_NEQ_OR_NAME_LIKE,
        D_GROUP_NEQ_AND_NAME_EQ, D_GROUP_NEQ_AND_NAME_NEQ, D_GROUP_NEQ_AND_NAME_LT, D_GROUP_NEQ_AND_NAME_GT, D_GROUP_NEQ_AND_NAME_GE, D_GROUP_NEQ_AND_NAME_LE, D_GROUP_NEQ_AND_NAME_LIKE,

        D_GROUP_LT_OR_NAME_EQ, D_GROUP_LT_OR_NAME_NEQ, D_GROUP_LT_OR_NAME_LT, D_GROUP_LT_OR_NAME_GT, D_GROUP_LT_OR_NAME_GE, D_GROUP_LT_OR_NAME_LE, D_GROUP_LT_OR_NAME_LIKE,
        D_GROUP_LT_AND_NAME_EQ, D_GROUP_LT_AND_NAME_NEQ, D_GROUP_LT_AND_NAME_LT, D_GROUP_LT_AND_NAME_GT, D_GROUP_LT_AND_NAME_GE, D_GROUP_LT_AND_NAME_LE, D_GROUP_LT_AND_NAME_LIKE,

        D_GROUP_GT_OR_NAME_EQ, D_GROUP_GT_OR_NAME_NEQ, D_GROUP_GT_OR_NAME_LT, D_GROUP_GT_OR_NAME_GT, D_GROUP_GT_OR_NAME_GE, D_GROUP_GT_OR_NAME_LE, D_GROUP_GT_OR_NAME_LIKE,
        D_GROUP_GT_AND_NAME_EQ, D_GROUP_GT_AND_NAME_NEQ, D_GROUP_GT_AND_NAME_LT, D_GROUP_GT_AND_NAME_GT, D_GROUP_GT_AND_NAME_GE, D_GROUP_GT_AND_NAME_LE, D_GROUP_GT_AND_NAME_LIKE,

        D_GROUP_LE_OR_NAME_EQ, D_GROUP_LE_OR_NAME_NEQ, D_GROUP_LE_OR_NAME_LT, D_GROUP_LE_OR_NAME_GT, D_GROUP_LE_OR_NAME_GE, D_GROUP_LE_OR_NAME_LE, D_GROUP_LE_OR_NAME_LIKE,
        D_GROUP_LE_AND_NAME_EQ, D_GROUP_LE_AND_NAME_NEQ, D_GROUP_LE_AND_NAME_LT, D_GROUP_LE_AND_NAME_GT, D_GROUP_LE_AND_NAME_GE, D_GROUP_LE_AND_NAME_LE, D_GROUP_LE_AND_NAME_LIKE,

        D_GROUP_GE_OR_NAME_EQ, D_GROUP_GE_OR_NAME_NEQ, D_GROUP_GE_OR_NAME_LT, D_GROUP_GE_OR_NAME_GT, D_GROUP_GE_OR_NAME_GE, D_GROUP_GE_OR_NAME_LE, D_GROUP_GE_OR_NAME_LIKE,
        D_GROUP_GE_AND_NAME_EQ, D_GROUP_GE_AND_NAME_NEQ, D_GROUP_GE_AND_NAME_LT, D_GROUP_GE_AND_NAME_GT, D_GROUP_GE_AND_NAME_GE, D_GROUP_GE_AND_NAME_LE, D_GROUP_GE_AND_NAME_LIKE,

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        D_GROUP_EQ_OR_PHONE_EQ, D_GROUP_EQ_OR_PHONE_NEQ, D_GROUP_EQ_OR_PHONE_LT, D_GROUP_EQ_OR_PHONE_GT, D_GROUP_EQ_OR_PHONE_GE, D_GROUP_EQ_OR_PHONE_LE,
        D_GROUP_EQ_AND_PHONE_EQ, D_GROUP_EQ_AND_PHONE_NEQ, D_GROUP_EQ_AND_PHONE_LT, D_GROUP_EQ_AND_PHONE_GT, D_GROUP_EQ_AND_PHONE_GE, D_GROUP_EQ_AND_PHONE_LE,

        D_GROUP_NEQ_OR_PHONE_EQ, D_GROUP_NEQ_OR_PHONE_NEQ, D_GROUP_NEQ_OR_PHONE_LT, D_GROUP_NEQ_OR_PHONE_GT, D_GROUP_NEQ_OR_PHONE_GE, D_GROUP_NEQ_OR_PHONE_LE,
        D_GROUP_NEQ_AND_PHONE_EQ, D_GROUP_NEQ_AND_PHONE_NEQ, D_GROUP_NEQ_AND_PHONE_LT, D_GROUP_NEQ_AND_PHONE_GT, D_GROUP_NEQ_AND_PHONE_GE, D_GROUP_NEQ_AND_PHONE_LE,

        D_GROUP_LT_OR_PHONE_EQ, D_GROUP_LT_OR_PHONE_NEQ, D_GROUP_LT_OR_PHONE_LT, D_GROUP_LT_OR_PHONE_GT, D_GROUP_LT_OR_PHONE_GE, D_GROUP_LT_OR_PHONE_LE,
        D_GROUP_LT_AND_PHONE_EQ, D_GROUP_LT_AND_PHONE_NEQ, D_GROUP_LT_AND_PHONE_LT, D_GROUP_LT_AND_PHONE_GT, D_GROUP_LT_AND_PHONE_GE, D_GROUP_LT_AND_PHONE_LE,

        D_GROUP_GT_OR_PHONE_EQ, D_GROUP_GT_OR_PHONE_NEQ, D_GROUP_GT_OR_PHONE_LT, D_GROUP_GT_OR_PHONE_GT, D_GROUP_GT_OR_PHONE_GE, D_GROUP_GT_OR_PHONE_LE,
        D_GROUP_GT_AND_PHONE_EQ, D_GROUP_GT_AND_PHONE_NEQ, D_GROUP_GT_AND_PHONE_LT, D_GROUP_GT_AND_PHONE_GT, D_GROUP_GT_AND_PHONE_GE, D_GROUP_GT_AND_PHONE_LE,

        D_GROUP_LE_OR_PHONE_EQ, D_GROUP_LE_OR_PHONE_NEQ, D_GROUP_LE_OR_PHONE_LT, D_GROUP_LE_OR_PHONE_GT, D_GROUP_LE_OR_PHONE_GE, D_GROUP_LE_OR_PHONE_LE,
        D_GROUP_LE_AND_PHONE_EQ, D_GROUP_LE_AND_PHONE_NEQ, D_GROUP_LE_AND_PHONE_LT, D_GROUP_LE_AND_PHONE_GT, D_GROUP_LE_AND_PHONE_GE, D_GROUP_LE_AND_PHONE_LE,

        D_GROUP_GE_OR_PHONE_EQ, D_GROUP_GE_OR_PHONE_NEQ, D_GROUP_GE_OR_PHONE_LT, D_GROUP_GE_OR_PHONE_GT, D_GROUP_GE_OR_PHONE_GE, D_GROUP_GE_OR_PHONE_LE,
        D_GROUP_GE_AND_PHONE_EQ, D_GROUP_GE_AND_PHONE_NEQ, D_GROUP_GE_AND_PHONE_LT, D_GROUP_GE_AND_PHONE_GT, D_GROUP_GE_AND_PHONE_GE, D_GROUP_GE_AND_PHONE_LE,


    };

    void op_delete(command &cmd)
    {
        d_command_funcs res = D_ALL;
        li.current = li.head;

        switch (cmd.c_group)
        {
            case COND_NONE:
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_ALL;
                        break;
                    case EQ:
                        res = D_NAME_EQ;
                        break;
                    case NEQ:
                        res = D_NAME_NEQ;
                        break;
                    case LT:
                        res = D_NAME_LT;
                        break;
                    case GT:
                        res = D_NAME_GT;
                        break;
                    case LE:
                        res = D_NAME_LE;
                        break;
                    case GE:
                        res = D_NAME_GE;
                        break;
                    case LIKE:
                        res = D_NAME_LIKE;
                        break;
                }
                    break;
                    //c_phone=
                case EQ:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_PHONE_EQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 1\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 2\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 3\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 4\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 5\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 6\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_EQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_PHONE_EQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed\n 7");
                            break;
                    }
                        break;
                }
                    break;


                    //c_phone=
                case NEQ:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_PHONE_NEQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 8\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 9\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 10\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 11\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 12\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 13\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_NEQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_PHONE_NEQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 14\n");
                            break;
                    }
                        break;
                }
                    break;







                    //c_phone=
                case LT:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_PHONE_LT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 15\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 16\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 17\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 18\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 19\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 20\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_PHONE_LT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 21\n");
                            break;
                    }
                        break;
                }
                    break;




                    //c_phone=
                case GT:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_PHONE_GT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 22\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 23\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 24\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 25\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 26\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 27\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_PHONE_GT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 28\n");
                            break;
                    }
                        break;
                }
                    break;





                    //c_phone=
                case LE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_PHONE_LE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 29\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 30\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 31\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 32\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 33\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 34\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_LE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_PHONE_LE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 35\n");
                            break;
                    }
                        break;
                }
                    break;





                    //c_phone=
                case GE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_PHONE_GE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 36\n");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 37\n");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 38\n");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 39\n");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 40\n");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 41\n");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_PHONE_GE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_PHONE_GE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("none cond while it needed 42\n");
                            break;
                    }
                        break;
                }
                    break;





                    //c_phone=
                case LIKE:
                    printf("Like with integers 1\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = COND_NONE

            case EQ:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_GROUP_EQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 43");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 44");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 45");
                            break;
                    }
                        break;
                    case GT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 46");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 47");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 48");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_EQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_GROUP_EQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 49");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is EQ
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_EQ_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = D_GROUP_EQ_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 50");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_EQ_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = D_GROUP_EQ_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 51");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_EQ_OR_PHONE_LT;
                        break;
                    case AND:
                        res = D_GROUP_EQ_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 52");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_EQ_OR_PHONE_GT;
                        break;
                    case AND:
                        res = D_GROUP_EQ_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 53");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_EQ_OR_PHONE_LE;
                        break;
                    case AND:
                        res = D_GROUP_EQ_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 54");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_EQ_OR_PHONE_GE;
                        break;
                    case AND:
                        res = D_GROUP_EQ_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 55");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 2\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = EQ


            case NEQ:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_GROUP_NEQ;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 56");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 57");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 58");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 59");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 60");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 61");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_NEQ_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_GROUP_NEQ_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 62");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is NEQ
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_NEQ_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = D_GROUP_NEQ_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 63");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_NEQ_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = D_GROUP_NEQ_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 64");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_NEQ_OR_PHONE_LT;
                        break;
                    case AND:
                        res = D_GROUP_NEQ_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 65");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_NEQ_OR_PHONE_GT;
                        break;
                    case AND:
                        res = D_GROUP_NEQ_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 66");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_NEQ_OR_PHONE_LE;
                        break;
                    case AND:
                        res = D_GROUP_NEQ_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 67");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_NEQ_OR_PHONE_GE;
                        break;
                    case AND:
                        res = D_GROUP_NEQ_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 68");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 3\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = NEQ




            case LT:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_GROUP_LT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 69");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 70");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 71");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 72");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 73");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 74");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_GROUP_LT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 75");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is LT
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 76");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 77");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_LT;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 78");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_GT;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 79");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_LE;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 80");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_GE;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 81");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 5\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = LT


            case GT:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_GROUP_GT;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 82");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 83");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 84");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 85");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 86");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 87");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GT_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_GROUP_GT_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 88");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is GT
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GT_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = D_GROUP_GT_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 89");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GT_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = D_GROUP_GT_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 90");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GT_OR_PHONE_LT;
                        break;
                    case AND:
                        res = D_GROUP_GT_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 91");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LT_OR_PHONE_GT;
                        break;
                    case AND:
                        res = D_GROUP_LT_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 92");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GT_OR_PHONE_LE;
                        break;
                    case AND:
                        res = D_GROUP_GT_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 93");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GT_OR_PHONE_GE;
                        break;
                    case AND:
                        res = D_GROUP_GT_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 94");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 4\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = GT


            case LE:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_GROUP_LE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 95");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 96");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 97");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 98");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 99");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 100");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_LE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_GROUP_LE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 101");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is LE
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LE_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = D_GROUP_LE_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 102");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LE_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = D_GROUP_LE_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 103");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LE_OR_PHONE_LT;
                        break;
                    case AND:
                        res = D_GROUP_LE_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 104");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LE_OR_PHONE_GT;
                        break;
                    case AND:
                        res = D_GROUP_LE_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 105");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LE_OR_PHONE_LE;
                        break;
                    case AND:
                        res = D_GROUP_LE_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 106");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_LE_OR_PHONE_GE;
                        break;
                    case AND:
                        res = D_GROUP_LE_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 107");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 6\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = LE



            case GE:  //c_group =
                switch (cmd.c_phone)
            {
                case COND_NONE:
                    switch (cmd.c_name)
                {
                    case COND_NONE:
                        res = D_GROUP_GE;
                        break;
                    case EQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_EQ;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_EQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 108");
                            break;
                    }
                        break;
                    case NEQ:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_NEQ;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_NEQ;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 109");
                            break;
                    }
                        break;
                    case LT:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_LT;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_LT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 110");
                            break;
                    }
                        break;
                    case GT: //c_phone = cond_none
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_GT;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_GT;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 111");
                            break;
                    }
                        break;
                    case LE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_LE;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_LE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 112");
                            break;
                    }
                        break;
                    case GE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_GE;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_GE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 113");
                            break;
                    }
                        break;
                    case LIKE:
                        switch (cmd.op)
                    {
                        case OR:
                            res = D_GROUP_GE_OR_NAME_LIKE;
                            break;
                        case AND:
                            res = D_GROUP_GE_AND_NAME_LIKE;
                            break;
                        case OP_NONE:
                            res = D_ERROR;
                            printf("None cond where it needed 114");
                            break;
                    }
                        break;
                }
                    break;
                    //c_phone=
                case EQ: //and c_group is GE
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GE_OR_PHONE_EQ;
                        break;
                    case AND:
                        res = D_GROUP_GE_AND_PHONE_EQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 115");
                        break;
                }
                    break;

                    //c_phone=
                case NEQ:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GE_OR_PHONE_NEQ;
                        break;
                    case AND:
                        res = D_GROUP_GE_AND_PHONE_NEQ;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 116");
                        break;
                }
                    break;

                    //c_phone=
                case LT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GE_OR_PHONE_LT;
                        break;
                    case AND:
                        res = D_GROUP_GE_AND_PHONE_LT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 117");
                        break;
                }
                    break;

                    //c_phone=
                case GT:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GE_OR_PHONE_GT;
                        break;
                    case AND:
                        res = D_GROUP_GE_AND_PHONE_GT;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 118");
                        break;
                }
                    break;

                    //c_phone=
                case LE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GE_OR_PHONE_LE;
                        break;
                    case AND:
                        res = D_GROUP_GE_AND_PHONE_LE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 119");
                        break;
                }
                    break;

                    //c_phone=
                case GE:
                    switch (cmd.op)
                {
                    case OR:
                        res = D_GROUP_GE_OR_PHONE_GE;
                        break;
                    case AND:
                        res = D_GROUP_GE_AND_PHONE_GE;
                        break;
                    case OP_NONE:
                        res = D_ERROR;
                        printf("None cond where it needed 120");
                        break;
                }
                    break;

                    //c_phone=
                case LIKE:
                    printf("Like with integers 7\n");
                    res = D_ERROR;
                    break;
            }
                break; //end c_group = GE

            case LIKE:
                printf("TERRIBLE ERROR, \"like\" with integers\n");
                break;
        } //end switch (cmd.group)

        if (!li.head) { printf("Selection on an empty set\n"); return; }
        li.current = li.head;

        command cmd1;

        //the command is stored is res now. What shall we do?
        switch (res) //SURE
        {
                //case ERROR: is below, it does not happen often
            case D_ALL:
                //~database2();
                break;

                ///-----------------------------------------------------
            case D_NAME_EQ:
                delete_name_EQ(cmd);
                break;

            case D_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;

            case D_NAME_LT:
                while(li.current)
                { if (cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;

            case D_NAME_GT:
                while(li.current)
                { if (cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;

            case D_NAME_LE:
                while(li.current)
                { if (cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_NAME_GE:
                while(li.current)
                { if (cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;

                //--phone only

            case D_PHONE_EQ:
                delete_phone_EQ(cmd);
                break;
            case D_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;

                //--group only

            case D_GROUP_EQ:
                cmd1.adjust_phone(cmd, 1234567);
                delete_group_EQ_and_phone_EQ(cmd1);
                cmd1.adjust_phone(cmd, 7654321);
                delete_group_EQ_and_phone_EQ(cmd1);
                break;
            case D_GROUP_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current)) li.delete_current(); else li.go_next();}
                break;


                // PHONE + NAME
                //------------------------phone_eq + name-----------------------------------------------------------------------------------------------

            case D_PHONE_EQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_PHONE_EQ_AND_NAME_EQ:
                delete_phone_EQ_and_name_EQ(cmd);
                break;
            case D_PHONE_EQ_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_EQ_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;

                //----------------- phone_neq + name --------------------------

            case D_PHONE_NEQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_PHONE_NEQ_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_NEQ_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_NEQ(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //-------------------- phone_lt + name -----------------------------


            case D_PHONE_LT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_PHONE_LT_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LT_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_LT(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //---------- PHONE_GT + NAME ----------------------------------------------



            case D_PHONE_GT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_PHONE_GT_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GT_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_GT(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;




                //---------------PHONE_GE + NAME

            case D_PHONE_GE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_PHONE_GE_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_GE_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_GE(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;

                //------------------------PHONE_LE + MANE



            case D_PHONE_LE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_PHONE_LE_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_PHONE_LE_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_PHONE_LE(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //-------------------------------------GROUP + NAME-----------------------------------------------------------------------------------------------------------------------------

                //------------------------phone_eq + name-----------------------------------------------------------------------------------------------

            case D_GROUP_EQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_EQ_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;

                //----------------- phone_neq + name --------------------------

            case D_GROUP_NEQ_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_NEQ_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //-------------------- phone_lt + name -----------------------------


            case D_GROUP_LT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_LT_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //---------- GROUP_GT + NAME ----------------------------------------------



            case D_GROUP_GT_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_GT_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;




                //---------------GROUP_GE + NAME

            case D_GROUP_GE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_GE_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;

                //------------------------GROUP_LE + MANE



            case D_GROUP_LE_OR_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_LE_AND_NAME_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_NAME_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_NAME_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_NAME_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_NAME_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_NAME_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_LE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_NAME_LIKE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_NAME_LIKE(li.current)) li.delete_current(); else li.go_next();}
                break;





                //---------------------GROUP + PHONE-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




                //------------------------GROUP_eq + PHONE-----------------------------------------------------------------------------------------------

            case D_GROUP_EQ_OR_PHONE_EQ:
                cmd1.adjust_phone(cmd, 1234567);
                delete_group_EQ_and_phone_EQ(cmd1);
                cmd1.adjust_phone(cmd, 7654321);
                delete_group_EQ_and_phone_EQ(cmd1);
                delete_phone_EQ(cmd);
                break;
            case D_GROUP_EQ_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) || cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_EQ_AND_PHONE_EQ:
                delete_group_EQ_and_phone_EQ(cmd);
                break;
            case D_GROUP_EQ_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_EQ_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_EQ(li.current) && cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
                //----------------- GROUP_neq + PHONE --------------------------

            case D_GROUP_NEQ_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) || cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_NEQ_AND_PHONE_EQ:
                //tr.delete_phone_EQ_and_group_NEQ(cmd, *this);
                break;
            case D_GROUP_NEQ_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_NEQ_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_NEQ(li.current) && cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //-------------------- GROUP_lt + PHONE -----------------------------


            case D_GROUP_LT_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) || cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;



            case D_GROUP_LT_AND_PHONE_EQ:
                //tr.delete_phone_EQ_and_group_LT(cmd, *this);
                break;
            case D_GROUP_LT_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LT_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LT(li.current) && cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


                //---------- GROUP_GT + PHONE ----------------------------------------------



            case D_GROUP_GT_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) || cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_GT_AND_PHONE_EQ:
                //tr.delete_phone_EQ_and_group_GT(cmd, *this);
                break;
            case D_GROUP_GT_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GT_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GT(li.current) && cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;



                //---------------GROUP_GE + PHONE

            case D_GROUP_GE_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) || cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_GE_AND_PHONE_EQ:
                //tr.delete_phone_EQ_and_group_GE(cmd, *this);
                break;
            case D_GROUP_GE_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_GE_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_GE(li.current) && cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;

                //------------------------GROUP_LE + MANE



            case D_GROUP_LE_OR_PHONE_EQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_EQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_OR_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) || cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_GROUP_LE_AND_PHONE_EQ:
                //tr.delete_phone_EQ_and_group_LE(cmd, *this);
                break;
            case D_GROUP_LE_AND_PHONE_NEQ:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_PHONE_NEQ(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_PHONE_LT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_PHONE_LT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_PHONE_GT:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_PHONE_GT(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_PHONE_GE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_PHONE_GE(li.current)) li.delete_current(); else li.go_next();}
                break;
            case D_GROUP_LE_AND_PHONE_LE:
                while(li.current)
                { if (cmd.cmp_GROUP_LE(li.current) && cmd.cmp_PHONE_LE(li.current)) li.delete_current(); else li.go_next();}
                break;


            case D_ERROR:
                printf("Error. OMG\n");
        } //end switch(res)
    }


    int operation(command &cmd)
    {
        switch (cmd.type)
        {
            case SELECT:
                op_select(cmd);
                //TODO
                return 1;
            case DELETE:
                op_delete(cmd);
                //TODO
                return 1;
            case INSERT:
                op_insert(cmd);
                //TODO
                return 1;
            case QUIT:
                //delete_list();
                printf("It quited\n");
                return 0;
            case STOP:
                //delete_list();
                printf("It stopped\n");
                return 0;
            case CMD_NONE:
                //printf("that was an empty string\n");
                return -1;
        }
        return 0;
    }
};

#endif //DATABASE2_H
