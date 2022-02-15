#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cstdlib>
#include <memory>
#include "razbor.h"
#include <cstring>
//#include "tree.h"

class list;
class tree;
class database2;

class list_node: public record
{
private:
    list_node *next = nullptr;
    list_node *prev = nullptr;

public:
    ~list_node() { next = prev = nullptr; }
    list_node() = default;
    friend class list;

    list_node *get_next() const
    { return next; }
    list_node *get_prev() const
    { return prev; }
    void set_next(list_node *next)
    { this->next = next; }
    
    int cmp_list_node(list_node* rhs)
    {
        if (this->group != rhs->group) return 0;
        if (this->phone != rhs->phone) return 0;
        if (strcmp(this->get_name(), rhs->get_name())) return 0;
        return 1;
    }
};


class list
{
private:
    list_node *head = nullptr;
    list_node *current = nullptr;
public:

    friend class tree;
    friend class database2;
    
    void set_head(list_node* node) { head = node; }
    
    void delete_next()
    {
        list_node* tmp = current->next;
        if (!tmp) { printf("No next element spotted\n"); return; }

        current->next = tmp->next;
        tmp->next = nullptr;
        delete tmp;
    }

    void delete_node(list_node *a)//in list
    {
        if (a == head && !a->next)
        {
            delete a;
            head = nullptr;
        }
        else
        {
            if (a == head)
            {
                head = a->next;
                head->prev = nullptr;
                delete a;
            }
            else
            {
                if (!a->next)
                {
                    a->prev->next = nullptr;
                    delete a;
                }
                else
                {
                    a->prev->next=a->next;
                    a->next->prev=a->prev;
                    delete a;
                }
            }
        }
    }


    int read_infinite (FILE *fp)
    {
        delete_list();
        list_node *next;
        int ret;

        next = new list_node();
        if (!next) return -1;

        ret = next->read(fp);
        if (ret < 0)
        {
            // incorrent read
            delete next;
            return -1;
        }

        if (ret > 0)
        {
            // end of file
            delete next;
            return 0;
        }

        head = current = next;

        for ( ; ; )
        {
            next = new list_node;
            if (!next)
                return 2;
            ret = next->read(fp);

            if (ret < 0)
            {
                //read error
                delete_list();
                return -1;
            }

            if (ret > 0)
            {
                delete next;
                return 0;
            }
            //if ret == 0
            current->next = next;
            next->prev = current;
            current = next;
            next = 0;
        }
        printf("How did I get here?\n");
        return 0; //never
    }

    void delete_list()
    {
        list_node *walker = head;
        while (walker)
        {
            list_node *walker_next = walker->next;
            delete walker;
            walker = walker_next;
        }
        head = current = nullptr;
    }

    void add_before_head(list_node *node)
    {
        if (!head) { head = node; return; }
        //head exists

        node->next = head;
        head->prev = node;
        head = node;
    }

    /*list_node* insert(const char* name, int phone, int group) //insertion in list(OLD!)
    {
        current = head;
        list_node * node;
        node = new list_node();

        if (!head)
        {
            node->init(name, phone, group);
            head = node;
        }
        else
        {
            head->prev = node;
            node->next = head;
            head = node;
            node->init(name, phone, group);
        }
        
        return node;
    }*/

    /*void op_insert(command &cmd, tree &tr)
    {
        list_node *node = new list_node();
        node->init(cmd.get_name(), cmd.phone, cmd.group);
        tr.insert(node, *this);
    }*/


    void delete_current() //deletes (and goes next!)
    {
        if (current == head && !(current->next))
        {
            delete current;
            head = current = nullptr;
        }
        else
        {
            if (current == head)
            {
                head = current->next;
                head->prev = nullptr;

                //list_node tmp = current;
                delete current;
                current = head;
            }
            else
            {
                if (!(current->next))
                {
                    list_node *tmp = current->prev;
                    current->prev->next = nullptr;
                    delete current;
                    current = tmp;
                }
                else
                {
                    list_node *tmp = current->prev;
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    delete current;
                    current = tmp;
                }
            }
        }
    }

#define MAX_PRINT 2000000
    void print_list(int depth = 0)
    {
        int counter = 0;
        printf("It started\n");
        if (!head) { for (int j = 0; j < depth; j++) printf("  "); printf("Empty list\n\n"); return; }
        list_node * current_ = head;
        for (int i = 0;current_ && i < MAX_PRINT; i++)
        {
            if (current_ == 0) break;
            for (int j = 0; j < depth; j++)
                //printf("%*s", depth*2, " "); //два пробела
                printf("  ");
            current_->print(); counter++;
            current_ = current_->next;
        }
        printf("print list ended; counter = %d\n", counter);
    }

    list() = default;
    ~list() { delete_list(); /* head = current = 0 is inside delete_list*/}

    int len();

    void print_current();
    void print(int depth = 0) { print_list(depth); }

    list_node* get_head() const
    { return head; }
    list_node* get_next() const
    { return current->next; }
    list_node* get_current() const
    { return current; }

    void go_next() { current = current->next; } //iterator
    //TODO maybe if (!current) current = head for safety purposes
    void operator++() { go_next(); }

    /*enum command_funcs
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

    void op_select(command &cmd, tree &tr)
    {
        (void) tr;
        command_funcs res;
        current = head;


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

	if (!head) { printf("Selection on an empty set\n"); return; }
		current = head;


	//the command is stored is res now. What shall we do?
	switch (res) //SURE
	{
	        //case ERROR: is below, it does not happen often
		case S_ALL:
			while(current->next)
			{ current->print(); go_next(); } //print all
			break;

	///-----------------------------------------------------
		case S_NAME_EQ:
			while(current)
			{ if (cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;

		case S_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;

		case S_NAME_LT:
			while(current)
			{ if (cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;

		case S_NAME_GT:
			while(current)
			{ if (cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;

		case S_NAME_LE:
			while(current)
			{ if (cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_NAME_GE:
			while(current)
			{ if (cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;

		//--phone only

		case S_PHONE_EQ:
            tr.select_cond_PHONE_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_NEQ:
            tr.select_cond_PHONE_NEQ(tr.root, cmd, *this);
			break;
		case S_PHONE_LT:
            tr.select_cond_PHONE_LT(tr.root, cmd, *this);
			break;
		case S_PHONE_GT:
			tr.select_cond_PHONE_GT(tr.root, cmd, *this);
			break;
		case S_PHONE_LE:
			tr.select_cond_PHONE_LE(tr.root, cmd, *this);
			break;
		case S_PHONE_GE:
			tr.select_cond_PHONE_GE(tr.root, cmd, *this);
			break;

		//--group only

		case S_GROUP_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current)) current->print(); go_next();}
			break;


			// PHONE + NAME
	//------------------------phone_eq + name-----------------------------------------------------------------------------------------------

		case S_PHONE_EQ_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_PHONE_EQ_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_PHONE_EQ_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_EQ_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_EQ_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_EQ_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_EQ_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_PHONE_EQ_AND_NAME_EQ:
            tr.select_cond_PHONE_EQ_AND_NAME_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_EQ_AND_NAME_NEQ:
            tr.select_cond_PHONE_EQ_AND_NAME_NEQ(tr.root, cmd, *this);
			break;
		case S_PHONE_EQ_AND_NAME_LT:
			tr.select_cond_PHONE_EQ_AND_NAME_LT(tr.root, cmd, *this);
			break;
		case S_PHONE_EQ_AND_NAME_GT:
			tr.select_cond_PHONE_EQ_AND_NAME_GT(tr.root, cmd, *this);
			break;
		case S_PHONE_EQ_AND_NAME_GE:
            tr.select_cond_PHONE_EQ_AND_NAME_GE(tr.root, cmd, *this);
			break;
		case S_PHONE_EQ_AND_NAME_LE:
            tr.select_cond_PHONE_EQ_AND_NAME_LE(tr.root, cmd, *this);
			break;
		case S_PHONE_EQ_AND_NAME_LIKE:
            tr.select_cond_PHONE_EQ_AND_NAME_LIKE(tr.root, cmd, *this);
			break;

		//----------------- phone_neq + name --------------------------

		case S_PHONE_NEQ_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_PHONE_NEQ_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_PHONE_NEQ_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_PHONE_NEQ_AND_NAME_EQ:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_AND_NAME_NEQ:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_AND_NAME_LT:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_AND_NAME_GT:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_AND_NAME_GE:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_AND_NAME_LE:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_NEQ_AND_NAME_LIKE:
            while(current) { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		//-------------------- phone_lt + name -----------------------------


		case S_PHONE_LT_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_PHONE_LT_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_PHONE_LT_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_LT_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_LT_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_LT_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_LT_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_PHONE_LT_AND_NAME_EQ:
			tr.select_cond_PHONE_LT_AND_NAME_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_LT_AND_NAME_NEQ:
			tr.select_cond_PHONE_LT_AND_NAME_NEQ(tr.root, cmd, *this);
			break;
		case S_PHONE_LT_AND_NAME_LT:
			tr.select_cond_PHONE_LT_AND_NAME_LT(tr.root, cmd, *this);
			break;
		case S_PHONE_LT_AND_NAME_GT:
			tr.select_cond_PHONE_LT_AND_NAME_GT(tr.root, cmd, *this);
			break;
		case S_PHONE_LT_AND_NAME_GE:
			tr.select_cond_PHONE_LT_AND_NAME_GE(tr.root, cmd, *this);
			break;
		case S_PHONE_LT_AND_NAME_LE:
			tr.select_cond_PHONE_LT_AND_NAME_LE(tr.root, cmd, *this);
			break;
		case S_PHONE_LT_AND_NAME_LIKE:
			tr.select_cond_PHONE_LT_AND_NAME_LIKE(tr.root, cmd, *this);
			break;


		//---------- PHONE_GT + NAME ----------------------------------------------



		case S_PHONE_GT_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_PHONE_GT_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_PHONE_GT_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_GT_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_GT_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_GT_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_GT_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_PHONE_GT_AND_NAME_EQ:
            tr.select_cond_PHONE_GT_AND_NAME_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_GT_AND_NAME_NEQ:
			tr.select_cond_PHONE_GT_AND_NAME_NEQ(tr.root, cmd, *this);
			break;
		case S_PHONE_GT_AND_NAME_LT:
			tr.select_cond_PHONE_GT_AND_NAME_LT(tr.root, cmd, *this);
			break;
		case S_PHONE_GT_AND_NAME_GT:
			tr.select_cond_PHONE_GT_AND_NAME_GT(tr.root, cmd, *this);
			break;
		case S_PHONE_GT_AND_NAME_GE:
			tr.select_cond_PHONE_GT_AND_NAME_GE(tr.root, cmd, *this);
			break;
		case S_PHONE_GT_AND_NAME_LE:
			tr.select_cond_PHONE_GT_AND_NAME_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_GT_AND_NAME_LIKE:
			tr.select_cond_PHONE_GT_AND_NAME_LIKE(tr.root, cmd, *this);
			break;




		//---------------PHONE_GE + NAME

		case S_PHONE_GE_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_PHONE_GE_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_PHONE_GE_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_GE_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_GE_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_GE_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_GE_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_PHONE_GE_AND_NAME_EQ:
			tr.select_cond_PHONE_GE_AND_NAME_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_GE_AND_NAME_NEQ:
            tr.select_cond_PHONE_GE_AND_NAME_NEQ(tr.root, cmd, *this);
			break;
		case S_PHONE_GE_AND_NAME_LT:
			tr.select_cond_PHONE_GE_AND_NAME_LT(tr.root, cmd, *this);
			break;
		case S_PHONE_GE_AND_NAME_GT:
			tr.select_cond_PHONE_GE_AND_NAME_GT(tr.root, cmd, *this);
			break;
		case S_PHONE_GE_AND_NAME_GE:
			tr.select_cond_PHONE_GE_AND_NAME_GE(tr.root, cmd, *this);
			break;
		case S_PHONE_GE_AND_NAME_LE:
			tr.select_cond_PHONE_GE_AND_NAME_LE(tr.root, cmd, *this);
			break;
		case S_PHONE_GE_AND_NAME_LIKE:
			tr.select_cond_PHONE_GE_AND_NAME_LIKE(tr.root, cmd, *this);
			break;

		//------------------------PHONE_LE + MANE



		case S_PHONE_LE_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_PHONE_LE_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_PHONE_LE_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_PHONE_LE_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_PHONE_LE_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_PHONE_LE_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_PHONE_LE_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_PHONE_LE_AND_NAME_EQ:
			tr.select_cond_PHONE_GE_AND_NAME_EQ(tr.root, cmd, *this);
			break;
		case S_PHONE_LE_AND_NAME_NEQ:
			tr.select_cond_PHONE_GE_AND_NAME_NEQ(tr.root, cmd, *this);
			break;
		case S_PHONE_LE_AND_NAME_LT:
			tr.select_cond_PHONE_GE_AND_NAME_LT(tr.root, cmd, *this);
			break;
		case S_PHONE_LE_AND_NAME_GT:
			tr.select_cond_PHONE_GE_AND_NAME_GT(tr.root, cmd, *this);
			break;
		case S_PHONE_LE_AND_NAME_GE:
			tr.select_cond_PHONE_GE_AND_NAME_GE(tr.root, cmd, *this);
			break;
		case S_PHONE_LE_AND_NAME_LE:
			tr.select_cond_PHONE_GE_AND_NAME_LE(tr.root, cmd, *this);
			break;
		case S_PHONE_LE_AND_NAME_LIKE:
			tr.select_cond_PHONE_GE_AND_NAME_LIKE(tr.root, cmd, *this);
			break;


	//-------------------------------------GROUP + NAME-----------------------------------------------------------------------------------------------------------------------------

		//------------------------phone_eq + name-----------------------------------------------------------------------------------------------

		case S_GROUP_EQ_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_EQ_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_EQ_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_GROUP_EQ_AND_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;

		//----------------- phone_neq + name --------------------------

		case S_GROUP_NEQ_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_NEQ_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_NEQ_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_GROUP_NEQ_AND_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		//-------------------- phone_lt + name -----------------------------


		case S_GROUP_LT_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_LT_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_LT_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_GROUP_LT_AND_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		//---------- GROUP_GT + NAME ----------------------------------------------



		case S_GROUP_GT_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_GT_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_GT_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_GROUP_GT_AND_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;




		//---------------GROUP_GE + NAME

		case S_GROUP_GE_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_GE_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_GE_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_GROUP_GE_AND_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;

		//------------------------GROUP_LE + MANE



		case S_GROUP_LE_OR_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_LE_OR_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_LE_OR_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;


		case S_GROUP_LE_AND_NAME_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_NAME_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_NAME_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_NAME_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_NAME_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_NAME_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_LE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_NAME_LIKE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_LIKE(current)) current->print(); go_next();}
			break;





	//---------------------GROUP + PHONE-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------







	//------------------------GROUP_eq + PHONE-----------------------------------------------------------------------------------------------

		case S_GROUP_EQ_OR_PHONE_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_EQ_OR_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_EQ_OR_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_OR_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		case S_GROUP_EQ_AND_PHONE_EQ:
			tr.select_cond_PHONE_EQ_AND_GROUP_EQ(tr.root, cmd, *this);
			break;
		case S_GROUP_EQ_AND_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_EQ_AND_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
			break;
		//----------------- GROUP_neq + PHONE --------------------------

		case S_GROUP_NEQ_OR_PHONE_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_NEQ_OR_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_NEQ_OR_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_OR_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		case S_GROUP_NEQ_AND_PHONE_EQ:
            tr.select_cond_PHONE_EQ_AND_GROUP_NEQ(tr.root, cmd, *this);
			break;
		case S_GROUP_NEQ_AND_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_NEQ_AND_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		//-------------------- GROUP_lt + PHONE -----------------------------


		case S_GROUP_LT_OR_PHONE_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_LT_OR_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_LT_OR_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_OR_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;



		case S_GROUP_LT_AND_PHONE_EQ:
			tr.select_cond_PHONE_EQ_AND_GROUP_LT(tr.root, cmd, *this);
			break;
		case S_GROUP_LT_AND_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LT_AND_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		//---------- GROUP_GT + PHONE ----------------------------------------------



		case S_GROUP_GT_OR_PHONE_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_GT_OR_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_GT_OR_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_OR_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		case S_GROUP_GT_AND_PHONE_EQ:
			tr.select_cond_PHONE_EQ_AND_GROUP_GT(tr.root, cmd, *this);
			break;
		case S_GROUP_GT_AND_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GT_AND_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;



		//---------------GROUP_GE + PHONE

		case S_GROUP_GE_OR_PHONE_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_GE_OR_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_GE_OR_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_OR_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		case S_GROUP_GE_AND_PHONE_EQ:
			tr.select_cond_PHONE_EQ_AND_GROUP_GE(tr.root, cmd, *this);
			break;
		case S_GROUP_GE_AND_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_GE_AND_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;

		//------------------------GROUP_LE + PHONE



		case S_GROUP_LE_OR_PHONE_EQ:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_EQ(current)) current->print(); go_next();}
		     	break;
	        case S_GROUP_LE_OR_PHONE_NEQ:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
		     	break;
		case S_GROUP_LE_OR_PHONE_LT:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_LT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_PHONE_GT:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_GT(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_PHONE_GE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_GE(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_OR_PHONE_LE:
			while(current)
			{ if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_LE(current)) current->print(); go_next();}
			break;


		case S_GROUP_LE_AND_PHONE_EQ:
			tr.select_cond_PHONE_EQ_AND_GROUP_LE(tr.root, cmd, *this);
			break;
		case S_GROUP_LE_AND_PHONE_NEQ:
            while(current)
            { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_PHONE_NEQ(current)) current->print(); go_next();}
			break;
		case S_GROUP_LE_AND_PHONE_LT:
			tr.select_cond_PHONE_LT_AND_GROUP_LE(tr.root, cmd, *this);
			break;
		case S_GROUP_LE_AND_PHONE_GT:
			tr.select_cond_PHONE_GT_AND_GROUP_LE(tr.root, cmd, *this);
			break;
		case S_GROUP_LE_AND_PHONE_GE:
			tr.select_cond_PHONE_GE_AND_GROUP_LE(tr.root, cmd, *this);
			break;
		case S_GROUP_LE_AND_PHONE_LE:
			tr.select_cond_PHONE_LE_AND_GROUP_LE(tr.root, cmd, *this);
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

    void op_delete(command &cmd, tree &tr)
    {
        (void) tr;
        d_command_funcs res;
        current = head;


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

	if (!head) { printf("Selection on an empty set\n"); return; }
		current = head;


	//the command is stored is res now. What shall we do?
	switch (res) //SURE
	{
	        //case ERROR: is below, it does not happen often
		case D_ALL:
                        delete_list();
			break;

	///-----------------------------------------------------
		case D_NAME_EQ:
			while(current)
                        { if (cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;

		case D_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;

		case D_NAME_LT:
			while(current)
                        { if (cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;

		case D_NAME_GT:
			while(current)
                        { if (cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;

		case D_NAME_LE:
			while(current)
                        { if (cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_NAME_GE:
			while(current)
                        { if (cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;

		//--phone only

		case D_PHONE_EQ:
            tr.delete_phone_EQ(cmd, *this);
			break;
		case D_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;

		//--group only

		case D_GROUP_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current)) delete_current(); else go_next();}
			break;


			// PHONE + NAME
	//------------------------phone_eq + name-----------------------------------------------------------------------------------------------

		case D_PHONE_EQ_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_PHONE_EQ_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_PHONE_EQ_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_PHONE_EQ_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_EQ_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;

		//----------------- phone_neq + name --------------------------

		case D_PHONE_NEQ_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_PHONE_NEQ_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_PHONE_NEQ_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_PHONE_NEQ_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_NEQ_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_NEQ(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		//-------------------- phone_lt + name -----------------------------


		case D_PHONE_LT_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_PHONE_LT_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_PHONE_LT_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_PHONE_LT_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LT_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_LT(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		//---------- PHONE_GT + NAME ----------------------------------------------



		case D_PHONE_GT_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_PHONE_GT_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_PHONE_GT_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_PHONE_GT_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GT_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_GT(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;




		//---------------PHONE_GE + NAME

		case D_PHONE_GE_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_PHONE_GE_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_PHONE_GE_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_PHONE_GE_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_GE_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_GE(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;

		//------------------------PHONE_LE + MANE



		case D_PHONE_LE_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_PHONE_LE_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_PHONE_LE_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_PHONE_LE_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_PHONE_LE_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_PHONE_LE(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


	//-------------------------------------GROUP + NAME-----------------------------------------------------------------------------------------------------------------------------

		//------------------------phone_eq + name-----------------------------------------------------------------------------------------------

		case D_GROUP_EQ_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_EQ_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_EQ_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_EQ_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;

		//----------------- phone_neq + name --------------------------

		case D_GROUP_NEQ_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_NEQ_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_NEQ_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_NEQ_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		//-------------------- phone_lt + name -----------------------------


		case D_GROUP_LT_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_LT_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_LT_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_LT_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		//---------- GROUP_GT + NAME ----------------------------------------------



		case D_GROUP_GT_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_GT_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_GT_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_GT_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;




		//---------------GROUP_GE + NAME

		case D_GROUP_GE_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_GE_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_GE_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_GE_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;

		//------------------------GROUP_LE + MANE



		case D_GROUP_LE_OR_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_LE_OR_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_LE_OR_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_LE_AND_NAME_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_NAME_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_NAME_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_NAME_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_NAME_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_NAME_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_LE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_NAME_LIKE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_NAME_LIKE(current)) delete_current(); else go_next();}
			break;





	//---------------------GROUP + PHONE-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




	//------------------------GROUP_eq + PHONE-----------------------------------------------------------------------------------------------

		case D_GROUP_EQ_OR_PHONE_EQ:
            tr.delete_phone_EQ(cmd, *this);
            while(current) { if (cmd.cmp_GROUP_EQ(current)) delete_current(); else go_next();}
            break;
	        case D_GROUP_EQ_OR_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_EQ_OR_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_OR_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) || cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_EQ_AND_PHONE_EQ:
			tr.delete_phone_EQ_and_group_EQ(cmd, *this);
			break;
		case D_GROUP_EQ_AND_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_EQ_AND_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_EQ(current) && cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
			break;
		//----------------- GROUP_neq + PHONE --------------------------

		case D_GROUP_NEQ_OR_PHONE_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_NEQ_OR_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_NEQ_OR_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_OR_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) || cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_NEQ_AND_PHONE_EQ:
			tr.delete_phone_EQ_and_group_NEQ(cmd, *this);
			break;
		case D_GROUP_NEQ_AND_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_NEQ_AND_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_NEQ(current) && cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		//-------------------- GROUP_lt + PHONE -----------------------------


		case D_GROUP_LT_OR_PHONE_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_LT_OR_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_LT_OR_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_OR_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) || cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;



		case D_GROUP_LT_AND_PHONE_EQ:
			tr.delete_phone_EQ_and_group_LT(cmd, *this);
			break;
		case D_GROUP_LT_AND_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LT_AND_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LT(current) && cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		//---------- GROUP_GT + PHONE ----------------------------------------------



		case D_GROUP_GT_OR_PHONE_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_GT_OR_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_GT_OR_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_OR_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) || cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_GT_AND_PHONE_EQ:
			tr.delete_phone_EQ_and_group_GT(cmd, *this);
			break;
		case D_GROUP_GT_AND_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GT_AND_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GT(current) && cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;



		//---------------GROUP_GE + PHONE

		case D_GROUP_GE_OR_PHONE_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_GE_OR_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_GE_OR_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_OR_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) || cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_GE_AND_PHONE_EQ:
			tr.delete_phone_EQ_and_group_GE(cmd, *this);
			break;
		case D_GROUP_GE_AND_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_GE_AND_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_GE(current) && cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;

		//------------------------GROUP_LE + MANE



		case D_GROUP_LE_OR_PHONE_EQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_EQ(current)) delete_current(); else go_next();}
		     	break;
	        case D_GROUP_LE_OR_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
		     	break;
		case D_GROUP_LE_OR_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_OR_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) || cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


		case D_GROUP_LE_AND_PHONE_EQ:
			tr.delete_phone_EQ_and_group_LE(cmd, *this);
			break;
		case D_GROUP_LE_AND_PHONE_NEQ:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_PHONE_NEQ(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_PHONE_LT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_PHONE_LT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_PHONE_GT:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_PHONE_GT(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_PHONE_GE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_PHONE_GE(current)) delete_current(); else go_next();}
			break;
		case D_GROUP_LE_AND_PHONE_LE:
			while(current)
                        { if (cmd.cmp_GROUP_LE(current) && cmd.cmp_PHONE_LE(current)) delete_current(); else go_next();}
			break;


	case D_ERROR:
		printf("Error. OMG\n");
	} //end switch(res)
    }


    int operation(command &cmd, tree &tr)
    {
        switch (cmd.type)
        {
            case SELECT:
                op_select(cmd, tr);
                //TODO
                return 1;
            case DELETE:
                op_delete(cmd, tr);
                //TODO
                return 1;
            case INSERT:
                op_insert(cmd, tr);
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
};*/

    
    
    
}; //end of class list
//-------end of list.h ------------------

#endif //LIST_H
