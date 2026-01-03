#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "parser.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

class list;
class tree;
class database;

class list_node : public record {
private:
  list_node *next = nullptr;
  list_node *prev = nullptr;

public:
  ~list_node() { next = prev = nullptr; }
  list_node() = default;
  friend class list;

  list_node *get_next() const { return next; }
  list_node *get_prev() const { return prev; }
  void set_next(list_node *next) { this->next = next; }

  int cmp_list_node(list_node *rhs) {
    if (this->group != rhs->group)
      return 0;
    if (this->phone != rhs->phone)
      return 0;
    if (strcmp(this->get_name(), rhs->get_name()))
      return 0;
    return 1;
  }
};

class list {
private:
  list_node *head = nullptr;
  list_node *current = nullptr;

public:
  friend class tree;
  friend class database;

  void set_head(list_node *node) { head = node; }

  void delete_next() {
    list_node *tmp = current->next;
    if (!tmp) {
      printf("No next element spotted\n");
      return;
    }

    current->next = tmp->next;
    tmp->next = nullptr;
    delete tmp;
  }

  void delete_node(list_node *a) {
    if (a == head && !a->next) {
      delete a;
      head = nullptr;
    } else {
      if (a == head) {
        head = a->next;
        head->prev = nullptr;
        delete a;
      } else {
        if (!a->next) {
          a->prev->next = nullptr;
          delete a;
        } else {
          a->prev->next = a->next;
          a->next->prev = a->prev;
          delete a;
        }
      }
    }
  }

  int read_infinite(FILE *fp) {
    delete_list();
    list_node *next;
    int ret;

    next = new list_node();
    if (!next)
      return -1;

    ret = next->read(fp);
    if (ret < 0) {
      delete next;
      return -1;
    }

    if (ret > 0) {
      delete next;
      return 0;
    }

    head = current = next;

    for (;;) {
      next = new list_node;
      if (!next)
        return 2;
      ret = next->read(fp);

      if (ret < 0) {
        delete_list();
        return -1;
      }

      if (ret > 0) {
        delete next;
        return 0;
      }
      current->next = next;
      next->prev = current;
      current = next;
      next = 0;
    }
    return 0;
  }

  void delete_list() {
    list_node *walker = head;
    while (walker) {
      list_node *walker_next = walker->next;
      delete walker;
      walker = walker_next;
    }
    head = current = nullptr;
  }

  void add_before_head(list_node *node) {
    if (!head) {
      head = node;
      return;
    }
    node->next = head;
    head->prev = node;
    head = node;
  }

  void delete_current() {
    if (current == head && !(current->next)) {
      delete current;
      head = current = nullptr;
    } else {
      if (current == head) {
        head = current->next;
        head->prev = nullptr;
        delete current;
        current = head;
      } else {
        if (!(current->next)) {
          list_node *tmp = current->prev;
          current->prev->next = nullptr;
          delete current;
          current = tmp;
        } else {
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
  void print_list(int depth = 0) {
    int counter = 0;
    printf("It started\n");
    if (!head) {
      for (int j = 0; j < depth; j++)
        printf("  ");
      printf("Empty list\n\n");
      return;
    }
    list_node *current_ = head;
    for (int i = 0; current_ && i < MAX_PRINT; i++) {
      if (current_ == 0)
        break;
      for (int j = 0; j < depth; j++)
        printf("  ");
      current_->print();
      counter++;
      current_ = current_->next;
    }
    printf("print list ended; counter = %d\n", counter);
  }
#undef MAX_PRINT

  list() = default;
  ~list() { delete_list(); }

  int len();

  void print_current();
  void print(int depth = 0) { print_list(depth); }

  list_node *get_head() const { return head; }
  list_node *get_next() const { return current->next; }
  list_node *get_current() const { return current; }

  void go_next() { current = current->next; }
  void operator++() { go_next(); }
};

#endif // LINKED_LIST_H
