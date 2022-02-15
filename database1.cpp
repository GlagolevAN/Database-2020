#include "database1.h"

//----------pointer_list

void pointer_list::add_before_head(list_node *data)
{
    pointer_list_node* node = new pointer_list_node();
    node->pointer = data;//init

    if (!head) { head = node; return; }

    //head exists
    node->next = head;
    head->prev = node;
    head = node;
}

void pointer_list::add_head(list_node* node)
{
    head = new pointer_list_node();
    head->pointer = node;
}

void pointer_list::delete_list()
{
    pointer_list_node *walker = head;
    while (walker)
    {
        pointer_list_node *walker_next = walker->next;
        delete walker;
        walker = walker_next;
    }
}

void pointer_list::delete_node(pointer_list_node *a)//in list
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

void pointer_list::killer(list_node* node)
{
    pointer_list_node* walkman = head;

    while(walkman)
    {
        if (node->cmp_list_node(walkman->pointer)) { delete_node(walkman); return; }
        walkman = walkman->next;
    }
}


list_node* pointer_list::delete_name_EQ(command& cmd)
{
    pointer_list_node* walkman = head;

    while(walkman)
    {
        if (cmd.cmp_NAME_EQ(walkman->pointer)) { list_node* tmp = walkman->pointer; delete_node(walkman); return tmp; }
        walkman = walkman->next;
    }
    return nullptr;
}

unsigned long int pointer_list::len()
{
    unsigned long int counter = 0;
    pointer_list_node * walkman = head;

    while (walkman)
    {
        counter++;
        walkman = walkman->next;
    }

    return counter;
}

/*list_node* pointer_list::delete_node_name_EQ(command& cmd)
{
    pointer_list_node* walkman = head;
    if_action = nullptr;
    while(walkman)
    {
        if (cmd.cmp_NAME_EQ(walkman->pointer)) { list_node* tmp = walkman->pointer; delete_node(walkman); return tmp; }
        walkman = walkman->next;
    }
    return nullptr;
}*/


//-----------database1


/*
 THEORY

./a.out ../a1000000_sort_n.txt 1000 <../commands_1000000_n.txt >out.txt
 (9 sec build-up)
 base hash 505 sec
 5381 hash 266 sec


 */


#ifndef ZACHET
//DISP a10^6 = 10^6
int database1::hash(const char* str)
{
    //printf("check\n");
    //if (!str) printf("empty_one\n");
    int sum = 0;
    for (int i = 0; str[i]; i++)
        sum += ((unsigned char)str[i] - (unsigned char)'A');

    return (sum % p);
}
#else
//5381 HASH
//disp a10^6 = 10^3
int database1::hash(const char *str)
{
    unsigned int hash = 5381;
    unsigned int c;

    while ( (c = *str++) )
        hash = ((hash << 5) + hash) + ((unsigned char)c); // hash * 33 + c

    //printf("hash = %d, p = %d, hash / p = %d\n", hash, p, hash % p);
    return (hash % p);
}

/*
//ROTATE HASH
//disp a10^6 = 1.5 * 10^3
int database1::hash(const char *str)
{
    unsigned int hash, i;
    unsigned int len = strlen(str);

    for (hash = len, i = 0; i < len; ++i)
        hash = (hash << 5) ^ (hash >> 27)
        ^
        (unsigned char)str[i];

    //printf("hash = %d, p = %d, hash / p = %d\n", hash, p, hash % p);
    return (hash % p);
}
 */

#endif

#include <ctime>
void database1::how_good_is_hash()
{
    double t = clock();
    double sum = 0., sum_sq = 0.;
    int tmp;
    //comments in this function is fake news
    //unsigned int non_zeros = 0;
    //avg_time represents the avg number of words with the hash value, where at least one word has such a value
    //activate ^^^ this^^^ to show that initial hash is bad
    for (int i = 0; i < p; i++)
    {
        tmp = pointer_li[i].len();
        printf("len[%d] = %d\n", i, tmp);
        //if (tmp) non_zeros++;
        sum += (double)tmp;
        sum_sq += ((double)tmp * (double)tmp);
    }

    printf("Sum = %.0lf, ", sum);
    printf("Exp = %.3lf\n", (double)sum / (double)p);
    //printf("weird value = %ld\n", sum_sq - sum * sum);
    printf("sum_sq / p = %.3lf,  (sum / p) ^ 2 = %.3lf\n",
           ((double)sum_sq / (double) p)
           ,
           ((double)(sum) / (double)p  * (double) (sum) / (double) (p)));
    printf("Disp = %.3lf\n",
           ((double)sum_sq / (double) p)
           -
           ((double)(sum) / (double)p  * (double) (sum) / (double) (p)));
    printf("Statistics' worktime = %.2lf\n\n", (clock() - t) / CLOCKS_PER_SEC);
}

int database1::insert(list_node* data)
{
    if (!tr.insert(data)) return 0;//repeated elem

    pointer_li[hash(data->get_name())].add_before_head(data);
    return 1;
}

int database1::big_insert(list_node* data)
{
    if (!tr.big_insert(data)) return 0;//repeated elem

    pointer_li[hash(data->get_name())].add_before_head(data);
    return 1;
}



void database1::init(list_node* node, int param)
{
    p = param; group = node->get_group();

    pointer_li = new pointer_list[p + 1];

    pointer_li[hash(node->get_name())].add_head(node);
    tr.add_root(node);
}

void database1::big_init(list_node* node) //for big db1
{
    pointer_li = new pointer_list[p + 1];

    pointer_li[hash(node->get_name())].add_head(node);
    tr.add_root(node);
}

void database1::select_cond_NAME_EQ(command& cmd)
{
    pointer_list_node* walkman = pointer_li[hash(cmd.get_name())].get_head();

    while(walkman)
    {
        if (cmd.cmp_NAME_EQ(walkman->get_pointer())) walkman->get_pointer()->print();
        walkman = walkman->get_next();
    }
}


void database1::killer(list_node* data_node)
{
    tr.big_killer(data_node);
    pointer_li[hash(data_node->get_name())].killer(data_node);
}



list_node* database1::delete_node_phone_EQ_and_name_EQ(command &cmd)
{
    if_action = nullptr;
    tr.delete_node_phone_EQ_and_name_EQ(cmd);
    if_action = tr.get_if_action();
    if (if_action)
    {
        pointer_li[hash(if_action->get_name())].killer(if_action);
        return if_action;
    }
    return nullptr;
}

list_node* database1::delete_node_phone_EQ(command &cmd)
{
    if_action = nullptr;
    tr.delete_node_phone_EQ(cmd);
    if_action = tr.get_if_action();
    if (if_action)
    {
        pointer_li[hash(if_action->get_name())].killer(if_action);
        return if_action;
    }
    return nullptr;
}

list_node* database1::big_delete_node_phone_EQ(command &cmd)
{
    if_action = nullptr;
    tr.big_delete_node_phone_EQ(cmd);
    if_action = tr.get_if_action();
    if (if_action)
    {
        pointer_li[hash(if_action->get_name())].killer(if_action);
        return if_action;
    }
    return nullptr;
}

list_node* database1::delete_node_name_EQ(command &cmd)
{
    //if_action = nullptr;
    if_action = pointer_li[hash(cmd.get_name())].delete_name_EQ(cmd);

    if (if_action)
    {
        tr.big_killer(if_action);
        return if_action;
    }
    return nullptr;
}


//------kilo_tree_node




//---------------kilo_tree

kilo_tree_node* kilo_tree::new_node(list_node* data)
{
    if_action = data;
    kilo_tree_node* node = new kilo_tree_node();
    node->data = new database1();
    node->data->init(data, p);//init ~~ insert in root

    //li.add_before_head(data);
    if (!root) root = node;

    return node;
}

int kilo_tree::get_height(kilo_tree_node *node)
{
    if (!node) return 0;
    return node->height;
}

int kilo_tree::get_balance(kilo_tree_node *node)
{
    if (!node) return 0;
    return get_height(node->left) - get_height(node->right);
}

kilo_tree_node *kilo_tree::right_rotate(kilo_tree_node *a)
{
    kilo_tree_node *b = a->left;
    kilo_tree_node *c = b->right;

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


kilo_tree_node *kilo_tree::left_rotate(kilo_tree_node *a)
{
    kilo_tree_node *b = a->right;
    kilo_tree_node *c = b->left;

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

kilo_tree_node* kilo_tree::insert(kilo_tree_node* node, list_node* data)
{   //yeah, data was not a good name

    if (!node)
        return(new_node(data));

    if (data->get_group() < node->data->group)//(data < node->data)
        node->left = insert(node->left, data);
    else if (data->get_group() > node->data->group)//(data > node->data)
        node->right = insert(node->right, data);
    else //group equals => insertion in db1
    {
        if (node->data->insert(data))
            if_action = data;
        return node;
    }
    //changes height of ancestor
    node->height = 1 + max_2(get_height(node->left), get_height(node->right));

    int balance = get_balance(node);

    //if ~balanced
    if (!balance || balance == -1 || balance == 1) return node;


    //if not balanced
    //L L Case
    if (balance > 1 && //data < node->left->data
        data->get_group() < node->left->data->group)
    {
        //printf("1\n");
        return right_rotate(node);
    }
    //R R Case
    if (balance < -1 && //data > node->right->data
        data->get_group() > node->right->data->group)
    {
        //printf("2\n");
        return left_rotate(node);
    }
    // L R Case
    if (balance > 1 && //data > node->left->data
        data->get_group() > node->left->data->group)
    {
        node->left = left_rotate(node->left);
        //printf("3\n");
        return right_rotate(node);
    }

    // R L Case
    if (balance < -1 && //data < node->right->data
        data->get_group() < node->right->data->group)
    {
        node->right = right_rotate(node->right);
        //printf("4\n");
        return left_rotate(node);
    }

    // never triggers
    printf("HOW\n");
    return node;
}


list_node* kilo_tree::insert(list_node *data)
{
    if_action = nullptr;
    insert(root, data);
    return if_action;
}

void kilo_tree::add_root(list_node* node) //adds root if the tree is empty
{
    insert(node);
}





void kilo_tree::delete_kilo_tree(kilo_tree_node* new_root)
{
    if (!new_root) return;
    if (new_root->left)
    {
        delete_kilo_tree(new_root->left); //nullify pointers
        new_root->left = nullptr;  //I guess it nullifies
    }
    if (new_root->right)
    {
        delete_kilo_tree(new_root->right);
        new_root->right = nullptr;
    }

    delete new_root->data;
    delete new_root;
}

void kilo_tree::delete_kilo_tree()
{
    delete_kilo_tree(root);
}

#define MAX_DEPTH 400

static int podschet = 0;
void kilo_tree::print_tree(kilo_tree_node* node, int depth)
{
    //curr и root передаются и не используются
    if (depth > MAX_DEPTH) return;

    if (node)
    {
        node->data->print(depth);podschet++;

        print_tree(node->left, depth + 1);
        print_tree(node->right, depth + 1);
    }
    else return;
}

void kilo_tree::print_tree()
{
    podschet = 0;
    if (!root) { printf("Empty tree\n\n"); return; }

    print_tree(root, 0);
    printf("chet =jerg %d\n", podschet);
    podschet = 0;
}
void kilo_tree::print() { print_tree(); }

void kilo_tree::select_group_EQ(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();

    if (node->data->group == cmd.group)
    {
        node->data->print();
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            select_group_EQ(node->right, cmd);
            return;
        }
        //GT
        select_group_EQ(node->left, cmd);
    }
}

void kilo_tree::select_group_EQ (command& cmd)
{
    select_group_EQ(root, cmd);
}

void kilo_tree::select_group_LT(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();

    if (node->data->group < cmd.group)
    {
        node->data->print();

        select_group_LT(node->right, cmd);
        select_group_LT(node->left, cmd);
    }
    else
    {
        select_group_LT(node->right, cmd);
    }
}

void kilo_tree::select_group_LT (command& cmd)
{
    select_group_LT(root, cmd);
}


void kilo_tree::select_phone_EQ (kilo_tree_node* node, command &cmd)
{
    if(!node) return;

    node->data->tr.select_cond_PHONE_EQ(cmd);

    select_phone_EQ(node->right, cmd);
    select_phone_EQ(node->left, cmd);
}

void kilo_tree::select_phone_EQ (command &cmd)
{
    select_phone_EQ(root, cmd);
}

void kilo_tree::select_name_EQ (kilo_tree_node* node, command &cmd)
{
    if(!node) return;

    node->data->select_cond_NAME_EQ(cmd);

    select_name_EQ(node->right, cmd);
    select_name_EQ(node->left, cmd);
}

void kilo_tree::select_name_EQ (command &cmd)
{
    select_name_EQ(root, cmd);
}

void kilo_tree::select_name_LIKE (kilo_tree_node* node, command &cmd)
{
    if(!node) return;

    node->data->tr.select_cond_NAME_LIKE(cmd);

    select_name_LIKE(node->right, cmd);
    select_name_LIKE(node->left, cmd);
}

void kilo_tree::select_name_LIKE (command &cmd)
{
    select_name_LIKE(root, cmd);
}


void kilo_tree::select_group_EQ_and_phone_EQ(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();

    if (node->data->group == cmd.group)
    {
        node->data->tr.select_cond_PHONE_EQ(cmd);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            select_group_EQ_and_phone_EQ(node->right, cmd);
            return;
        }
        //GT
        select_group_EQ_and_phone_EQ(node->left, cmd);
    }
}

void kilo_tree::select_group_EQ_and_phone_EQ (command& cmd)
{
    select_group_EQ_and_phone_EQ(root, cmd);
}

void kilo_tree::select_group_EQ_and_name_EQ(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();

    if (node->data->group == cmd.group)
    {
        node->data->tr.select_cond_NAME_EQ(cmd);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            select_group_EQ_and_name_EQ(node->right, cmd);
            return;
        }
        //GT
        select_group_EQ_and_name_EQ(node->left, cmd);
    }
}

void kilo_tree::select_group_EQ_and_name_EQ (command& cmd)
{
    select_group_EQ_and_name_EQ(root, cmd);
}

void kilo_tree::select_phone_EQ_and_name_EQ(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();


    node->data->tr.select_cond_PHONE_EQ_AND_NAME_EQ(cmd);

    select_phone_EQ_and_name_EQ(node->right, cmd);
    select_phone_EQ_and_name_EQ(node->left, cmd);

}

void kilo_tree::select_phone_EQ_and_name_EQ (command& cmd)
{
    select_phone_EQ_and_name_EQ(root, cmd);
}



void kilo_tree::select_group_EQ_and_phone_NEQ(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();

    if (node->data->group == cmd.group)
    {
        node->data->tr.select_cond_PHONE_NEQ(cmd);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            select_group_EQ_and_phone_NEQ(node->right, cmd);
            return;
        }
        //GT
        select_group_EQ_and_phone_NEQ(node->left, cmd);
    }
}

void kilo_tree::select_group_EQ_and_phone_NEQ (command& cmd)
{
    select_group_EQ_and_phone_NEQ(root, cmd);
}






void kilo_tree::select_group_EQ_and_phone_GE(kilo_tree_node* node, command& cmd)
{
    //printf("A\n");
    if (!node) return;
    //printf("B\n");
    //node->data->print();

    if (node->data->group == cmd.group)
    {
        node->data->tr.select_cond_PHONE_GE(cmd);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            select_group_EQ_and_phone_GE(node->right, cmd);
            return;
        }
        //GT
        select_group_EQ_and_phone_GE(node->left, cmd);
    }
}

void kilo_tree::select_group_EQ_and_phone_GE (command& cmd)
{
    select_group_EQ_and_phone_GE(root, cmd);
}

//--------------
void kilo_tree::delete_node_group_EQ_and_phone_EQ(kilo_tree_node* node, command& cmd)
{
    if (!node) return;

    if (node->data->group == cmd.group)
    {
        node->data->tr.delete_node_phone_EQ(cmd);
        if_action = node->data->tr.get_if_action();
        if (if_action) node->data->pointer_li[node->data->hash(if_action->get_name())].killer(if_action);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            delete_node_group_EQ_and_phone_EQ(node->right, cmd);
            return;
        }
        //GT
        delete_node_group_EQ_and_phone_EQ(node->left, cmd);
    }
}

void kilo_tree::delete_node_group_EQ_and_phone_EQ(command& cmd)
{ delete_node_group_EQ_and_phone_EQ(root, cmd); }

//kilo_tr.delete_node_group_EQ_and_name_EQ(cmd);
void kilo_tree::delete_node_group_EQ_and_name_EQ(kilo_tree_node* node, command& cmd)
{
    if (!node) return;

    if (node->data->group == cmd.group)
    {
        if_action = node->data->pointer_li[node->data->hash(if_action->get_name())].delete_name_EQ(cmd);
        //if_action = node->data->tr.get_if_action();
        if (if_action) node->data->tr.killer(if_action);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            select_group_EQ_and_phone_GE(node->right, cmd);
            return;
        }
        //GT
        select_group_EQ_and_phone_GE(node->left, cmd);
    }
}

void kilo_tree::delete_node_group_EQ_and_name_EQ(command& cmd)
{ delete_node_group_EQ_and_name_EQ(root, cmd); }




void kilo_tree::delete_node_group_EQ(kilo_tree_node* node, command& cmd)
{
    if (!node) return;

    if (node->data->group == cmd.group)
    {
        node->data->tr.killer_if_action();
        if_action = node->data->tr.get_if_action();
        if (if_action) node->data->pointer_li[node->data->hash(if_action->get_name())].killer(if_action);
    }
    else
    {
        if (node->data->group < cmd.group) // cmd. > node->data
        {
            delete_node_group_EQ(node->right, cmd);
            return;
        }
        //GT
        delete_node_group_EQ(node->left, cmd);
    }
}

void kilo_tree::delete_node_group_EQ(command& cmd)
{ delete_node_group_EQ(root, cmd); }



void kilo_tree::killer(kilo_tree_node* node, list_node* data_node)
{
    if (!node) return;

    if (node->data->group == data_node->group)
    {
        node->data->tr.killer(data_node);
        if_action = node->data->tr.get_if_action();
        if (if_action) node->data->pointer_li
            [node->data->hash(data_node->get_name())].killer(data_node);
    }
    else
    {
        if (node->data->group < data_node->group) // cmd. > node->data
        {
            killer(node->right, data_node);
            return;
        }
        //GT
        killer(node->left, data_node);
    }
}

void kilo_tree::killer(list_node* data_node) { killer(root, data_node); }







































































































































/*kilo_tree_node* kilo_tree::killer(tree_node* node, list_node* data)
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
 }*/

