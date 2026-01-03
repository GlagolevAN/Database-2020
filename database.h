#ifndef DATABASE_H
#define DATABASE_H

#include "database_index.h"
#include <functional>

class database {
private:
  list record_list;
  group_index_tree group_tree;
  record_index global_index;

  // Comparison function type for conditions
  using ConditionFunc = std::function<bool(command &, record *)>;

  // Get comparison function for a single field
  static ConditionFunc get_name_comparator(cond_type c) {
    switch (c) {
    case EQ:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_EQ(r); };
    case NEQ:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_NEQ(r); };
    case LT:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_LT(r); };
    case GT:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_GT(r); };
    case LE:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_LE(r); };
    case GE:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_GE(r); };
    case LIKE:
      return [](command &cmd, record *r) { return cmd.cmp_NAME_LIKE(r); };
    default:
      return nullptr;
    }
  }

  static ConditionFunc get_phone_comparator(cond_type c) {
    switch (c) {
    case EQ:
      return [](command &cmd, record *r) { return cmd.cmp_PHONE_EQ(r); };
    case NEQ:
      return [](command &cmd, record *r) { return cmd.cmp_PHONE_NEQ(r); };
    case LT:
      return [](command &cmd, record *r) { return cmd.cmp_PHONE_LT(r); };
    case GT:
      return [](command &cmd, record *r) { return cmd.cmp_PHONE_GT(r); };
    case LE:
      return [](command &cmd, record *r) { return cmd.cmp_PHONE_LE(r); };
    case GE:
      return [](command &cmd, record *r) { return cmd.cmp_PHONE_GE(r); };
    default:
      return nullptr;
    }
  }

  static ConditionFunc get_group_comparator(cond_type c) {
    switch (c) {
    case EQ:
      return [](command &cmd, record *r) { return cmd.cmp_GROUP_EQ(r); };
    case NEQ:
      return [](command &cmd, record *r) { return cmd.cmp_GROUP_NEQ(r); };
    case LT:
      return [](command &cmd, record *r) { return cmd.cmp_GROUP_LT(r); };
    case GT:
      return [](command &cmd, record *r) { return cmd.cmp_GROUP_GT(r); };
    case LE:
      return [](command &cmd, record *r) { return cmd.cmp_GROUP_LE(r); };
    case GE:
      return [](command &cmd, record *r) { return cmd.cmp_GROUP_GE(r); };
    default:
      return nullptr;
    }
  }

  // Combine two conditions with AND/OR operator
  static ConditionFunc combine_conditions(ConditionFunc f1, ConditionFunc f2,
                                          op_type op) {
    if (!f1)
      return f2;
    if (!f2)
      return f1;

    if (op == AND) {
      return [f1, f2](command &cmd, record *r) {
        return f1(cmd, r) && f2(cmd, r);
      };
    } else { // OR
      return [f1, f2](command &cmd, record *r) {
        return f1(cmd, r) || f2(cmd, r);
      };
    }
  }

  // Build condition from command - returns null if no condition
  ConditionFunc build_condition(command &cmd) {
    ConditionFunc name_cond = get_name_comparator(cmd.c_name);
    ConditionFunc phone_cond = get_phone_comparator(cmd.c_phone);
    ConditionFunc group_cond = get_group_comparator(cmd.c_group);

    ConditionFunc result = nullptr;

    // If we have phone and name conditions, combine them
    if (phone_cond && name_cond) {
      result = combine_conditions(phone_cond, name_cond, cmd.op);
    }
    // If we have group and name conditions, combine them
    else if (group_cond && name_cond) {
      result = combine_conditions(group_cond, name_cond, cmd.op);
    }
    // If we have group and phone conditions, combine them
    else if (group_cond && phone_cond) {
      result = combine_conditions(group_cond, phone_cond, cmd.op);
    }
    // Single condition cases
    else if (name_cond) {
      result = name_cond;
    } else if (phone_cond) {
      result = phone_cond;
    } else if (group_cond) {
      result = group_cond;
    }

    return result;
  }

  // Generic list traversal for SELECT
  void traverse_list_select(command &cmd, ConditionFunc condition) {
    record_list.current = record_list.head;
    while (record_list.current) {
      if (condition(cmd, record_list.current)) {
        record_list.current->print();
      }
      record_list.go_next();
    }
  }

  // Generic list traversal for DELETE
  void traverse_list_delete(command &cmd, ConditionFunc condition) {
    record_list.current = record_list.head;
    while (record_list.current) {
      if (condition(cmd, record_list.current)) {
        record_list.delete_current();
      } else {
        record_list.go_next();
      }
    }
  }

  // Try to use optimized select paths, returns true if handled
  bool try_optimized_select(command &cmd) {
    // Single condition optimizations - NAME only
    if (cmd.c_group == COND_NONE && cmd.c_phone == COND_NONE) {
      if (cmd.c_name == EQ) {
        group_tree.select_name_EQ(cmd); // Uses hash table
        return true;
      }
      if (cmd.c_name == LIKE) {
        group_tree.select_name_LIKE(cmd);
        return true;
      }
      return false;
    }

    // Single condition optimizations - PHONE only
    if (cmd.c_group == COND_NONE && cmd.c_name == COND_NONE) {
      if (cmd.c_phone == EQ) {
        global_index.get_tree().select_cond_PHONE_EQ(cmd);
        return true;
      }
      return false;
    }

    // Single condition optimizations - GROUP only (all comparison operators)
    if (cmd.c_phone == COND_NONE && cmd.c_name == COND_NONE) {
      switch (cmd.c_group) {
      case EQ:
        group_tree.select_group_EQ(cmd);
        return true;
      case LT:
        group_tree.select_group_LT(cmd);
        return true;
      case GT:
        group_tree.select_group_GT(cmd);
        return true;
      case LE:
        group_tree.select_group_LE(cmd);
        return true;
      case GE:
        group_tree.select_group_GE(cmd);
        return true;
      default:
        return false;
      }
    }

    // Combined condition optimizations (AND only for indexed operations)
    if (cmd.c_group == COND_NONE && cmd.c_phone == EQ && cmd.c_name == EQ &&
        cmd.op == AND) {
      group_tree.select_phone_EQ_and_name_EQ(cmd);
      return true;
    }

    if (cmd.c_phone == COND_NONE && cmd.c_group == EQ && cmd.c_name == EQ &&
        cmd.op == AND) {
      group_tree.select_group_EQ_and_name_EQ(cmd);
      return true;
    }

    if (cmd.c_name == COND_NONE && cmd.c_group == EQ && cmd.c_phone == EQ &&
        cmd.op == AND) {
      group_tree.select_group_EQ_and_phone_EQ(cmd);
      return true;
    }

    return false;
  }

  // Try to use optimized delete paths, returns true if handled
  bool try_optimized_delete(command &cmd) {
    // Single condition optimizations
    if (cmd.c_phone == COND_NONE && cmd.c_name == COND_NONE) {
      if (cmd.c_group == EQ) {
        group_tree.delete_node_group_EQ(cmd);
        return true;
      }
      return false;
    }

    // Combined condition optimizations
    if (cmd.c_name == COND_NONE && cmd.c_group == EQ && cmd.c_phone == EQ &&
        cmd.op == AND) {
      group_tree.delete_node_group_EQ_and_phone_EQ(cmd);
      return true;
    }

    if (cmd.c_phone == COND_NONE && cmd.c_group == EQ && cmd.c_name == EQ &&
        cmd.op == AND) {
      group_tree.delete_node_group_EQ_and_name_EQ(cmd);
      return true;
    }

    return false;
  }

public:
  database() = default;
  ~database() = default;

  void set_p(int value) {
    group_tree.set_p(value);
    global_index.set_p(value);
  }
  int get_p() { return group_tree.get_p(); }

  void how_good_is_hash() { global_index.how_good_is_hash(); }

  void print() {
    printf("list is fine:\n");
    record_list.print_list();
    printf("tree is fine:\n");
    group_tree.print();
    printf("database1:\n");
    global_index.print();
  }

  int read(FILE *fp) {
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

    group_tree.add_root(next);
    record_list.set_head(next);
    global_index.big_init(next);

    for (;;) {
      next = new list_node;
      if (!next)
        return 2;
      ret = next->read(fp);
      switch (ret) {
      case 0:
        break;
      case -1:
        printf("Cannot read\n");
        return -1;
      case 1:
        delete next;
        return 0;
      case -2:
        printf("Not enough memory\n");
        return 1;
      case 2:
        printf("Not enough memory\n");
        return 2;

      default:
        printf("bad stuff number %d\n", ret);
        return ret;
      }
      if (group_tree.insert(next)) {
        record_list.add_before_head(next);
        global_index.big_insert(next);
      }
    }

    return 0;
  }

  void op_insert(command &cmd) {
    list_node *node = new list_node();
    node->init(cmd.get_name(), cmd.phone, cmd.group);
    if (group_tree.insert(node)) {
      record_list.add_before_head(node);
      global_index.big_insert(node);
    }
  }

  // Main SELECT operation
  void op_select(command &cmd) {
    record_list.current = record_list.head;

    // Check for SELECT * (no conditions)
    if (cmd.c_group == COND_NONE && cmd.c_phone == COND_NONE &&
        cmd.c_name == COND_NONE) {
      while (record_list.current) {
        record_list.current->print();
        record_list.go_next();
      }
      printf("end of selection\n\n");
      return;
    }

    // Try optimized paths first
    if (try_optimized_select(cmd)) {
      printf("end of selection\n\n");
      return;
    }

    // Fall back to generic list traversal
    ConditionFunc condition = build_condition(cmd);
    if (condition) {
      traverse_list_select(cmd, condition);
    }

    printf("end of selection\n\n");
  }

  // Main DELETE operation - now drastically simprecord_listfied
  void op_delete(command &cmd) {
    record_list.current = record_list.head;

    // Check for DELETE * (no conditions) - delete all
    if (cmd.c_group == COND_NONE && cmd.c_phone == COND_NONE &&
        cmd.c_name == COND_NONE) {
      record_list.delete_list();
      return;
    }

    // Try optimized paths first
    if (try_optimized_delete(cmd)) {
      return;
    }

    // Fall back to generic list traversal
    ConditionFunc condition = build_condition(cmd);
    if (condition) {
      traverse_list_delete(cmd, condition);
    }
  }

  int operation(command &cmd) {
    switch (cmd.type) {
    case SELECT:
      op_select(cmd);
      return 1;
    case DELETE:
      op_delete(cmd);
      return 1;
    case INSERT:
      op_insert(cmd);
      return 1;
    case QUIT:
      printf("It quited\n");
      return 0;
    case STOP:
      printf("It stopped\n");
      return 0;
    case CMD_NONE:
      return -1;
    }
    return 0;
  }
};

#endif // DATABASE_H
