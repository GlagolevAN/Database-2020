#ifndef RAZBOR_H
#define RAZBOR_H

#include <iostream>
#include <cstdlib>
#include <memory>
#define LEN 890
//#include "list.h"
//#include "tree.h"


class tree;
class list;
class list_node;
class database2;
class parse;
class kilo_tree;


class record
{
private:
    std::unique_ptr<char[]> name;
    int phone = -1;
    int group = -1;
public:
    record() = default;

    void destroy(); //may be faulty
    ~record();
    void print(int depth = 0, FILE *out = stdout)
    {
        for (int i = 0; i < depth; i++) fprintf(out, "  ");
        //if (name)
        fprintf(out, "%s %d %d\n", get_name(), phone, group);
        //else fprintf(out, "\"Нулевой указатель\", телефон, группа\n");
    }
    int read(FILE *fp);
    const char* get_name() const
    { return (const char*) name.get(); }
    int get_phone() { return phone; }
    int get_group() { return group; }
    int init (const char*, int, int);
    int init_name(const char *n);

    //-COMPARATORS
    int operator==(const record& rhs);
    int spec_cmp_phone_lt(const record& rhs);
    int spec_cmp_phone_gt(const record& rhs);
    
    int big_spec_cmp_phone_lt(const record& rhs);
    int big_spec_cmp_phone_gt(const record& rhs);

    friend class command;
    friend class tree;
    friend class list;
    friend class list_node;
    friend class database2;
    friend class kilo_tree;
};


enum cmd_type
{ CMD_NONE, QUIT, STOP, SELECT, INSERT, DELETE };
enum cond_type
{ COND_NONE, EQ, NEQ, LT, GT, LE, GE, LIKE };
enum op_type
{ OP_NONE, AND, OR };

class command: public record
{
private:
    cmd_type type = CMD_NONE;
    cond_type c_name = COND_NONE;
    cond_type c_phone = COND_NONE;
    cond_type c_group = COND_NONE;
    op_type op = OP_NONE;
public:
    //construct and destruct
    command() = default;
    ~command() { type = CMD_NONE; c_name = COND_NONE; c_phone = COND_NONE; c_group = COND_NONE; op = OP_NONE;
            } //record destroys himself
    int parse_(char* string);
    //space chars are ' ' ';' '(' ')', also tab and enter
    friend class list_node;
    friend class list;
    friend class database2;
    
    //for like
    void init_table(); void delete_table();
    
    //for tests
    void adjustion(command&cmd) { init ("Student", cmd.phone, cmd.group); }
    void adjust_phone(command&cmd, int new_phone) { init (cmd.get_name(), new_phone, cmd.group); }
    //prints
    int print(FILE *out); //print in file
    void print_hat(FILE *out = stdout);
    void print_cmd(FILE *out = stdout); //print in console in a "beautiful" way
    void nullify();
    int parser(parse &);
    //void destroy();

    //for list; also helpers for tree
    int cmp_NAME_EQ(record*);
    int cmp_NAME_NEQ(record*);
    int cmp_NAME_LT(record*);
    int cmp_NAME_GT(record*);
    int cmp_NAME_LE(record*);
    int cmp_NAME_GE(record*);
    int cmp_NAME_LIKE(record*);

    int cmp_PHONE_EQ(record*);
    int cmp_PHONE_NEQ(record*);
    int cmp_PHONE_LT(record*);
    int cmp_PHONE_GT(record*);
    int cmp_PHONE_LE(record*);
    int cmp_PHONE_GE(record*);


    int cmp_GROUP_EQ(record*);
    int cmp_GROUP_NEQ(record*);
    int cmp_GROUP_LT(record*);
    int cmp_GROUP_GT(record*);
    int cmp_GROUP_LE(record*);
    int cmp_GROUP_GE(record*);

    //for tree
    //int spec_cmp_phone_gt(record*);

    //------------------------------------------------


};




class parse //входная строка
{
private:
    char** words = nullptr;
    int perstr = 0;//not forget to -1
public:
    void destroy();
    void print();
    parse() = default;
    ~parse() { destroy(); }
    friend class command;
    int init_words();
    int init(char *, char *, int *sp);
};



/*int if_less(const char *s1, const char *s2)
{
    if (strcmp(s1, s2) < 0) return 1; //condition holds true
    return 0;
}

int if_more(const char *s1, const char *s2)
{
    if (strcmp(s1, s2) > 0) return 1; //condition holds true
    return 0;
}

int if_equal(const char *s1, const char *s2)
{
    if (strcmp(s1, s2) == 0) return 1; //condition holds true
    return 0;
}

int if_not_equal(const char *s1, const char *s2)
{
    if (strcmp(s1, s2) != 0) return 1; //condition holds true
    return 0;
}

int if_no_more(const char *s1, const char *s2)
{
    if (strcmp(s1, s2) <= 0) return 1; //condition holds true
    return 0;
}

int if_no_less(const char *s1, const char *s2)
{
    if (strcmp(s1, s2) >= 0) return 1; //condition holds true
    return 0;
}

int if_like(const char *s1, const char *s2)
{
    printf("TODO TODO TODO\n");
    printf("Like is not ready\n");

    if (strcmp(s1, s2) < 0) return 1; //condition holds true
    return 0;
}*/


#endif //RAZBOR_H
