#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "linked_list.h"
#include "parser.h"

// Function pointer type for comparison functions in command class
typedef int (command::*CompareFunc)(record *);

static int schet = 0;

class tree;

class tree_node {
private:
  tree_node *left = nullptr;
  tree_node *right = nullptr;
  int height = 1;
  list_node *data = nullptr;

public:
  friend class tree;

  tree_node() = default;
  ~tree_node() {
    left = right = nullptr;
    height = 1;
    data = nullptr;
  }

  void set_left(tree_node *left) { this->left = left; }
  void set_right(tree_node *right) { this->right = right; }
  tree_node *get_left() const { return left; }
  tree_node *get_right() const { return right; }
  int get_height() const { return height; }
  list_node *get_data() const { return data; }
};

class tree {
private:
  tree_node *root = nullptr;
  tree_node *curr = nullptr;

  bool has_deleted = 0;
  list_node *if_action = nullptr;

public:
  list_node *get_if_action() const { return if_action; }

  int max_2(int lhs, int rhs) { return lhs > rhs ? lhs : rhs; }

  void add_root(list_node *node) {
    root = new tree_node();
    root->data = node;
  }

  tree_node *new_node(list_node *data) {
    if_action = data;
    tree_node *node = new tree_node();
    node->data = data;
    if (!root)
      root = node;
    return node;
  }

  int get_height(tree_node *node) {
    if (!node)
      return 0;
    return node->height;
  }

  int get_balance(tree_node *node) {
    if (!node)
      return 0;
    return get_height(node->left) - get_height(node->right);
  }

  tree_node *right_rotate(tree_node *a) {
    tree_node *b = a->left;
    tree_node *c = b->right;

    if (a == root)
      root = b;

    b->right = a;
    a->left = c;

    a->height = 1 + max_2(get_height(a->left), get_height(a->right));
    b->height = 1 + max_2(get_height(b->left), get_height(b->right));

    return b;
  }

  tree_node *left_rotate(tree_node *a) {
    tree_node *b = a->right;
    tree_node *c = b->left;

    if (a == root)
      root = b;

    b->left = a;
    a->right = c;

    a->height = 1 + max_2(get_height(a->left), get_height(a->right));
    b->height = 1 + max_2(get_height(b->left), get_height(b->right));

    return b;
  }

  tree_node *balansirovka(tree_node *node) {
    if (!node)
      return nullptr;

    node->height = 1 + max_2(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);

    if (!balance || balance == -1 || balance == 1)
      return node;

    if (balance > 1) {
      if (get_balance(node->left) >= 0)
        return right_rotate(node);
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }

    if (balance < -1) {
      if (get_balance(node->right) <= 0)
        return left_rotate(node);
      node->right = right_rotate(node->right);
      return left_rotate(node);
    }

    return node;
  }

  // ==================== INSERT OPERATIONS ====================

  tree_node *insert(tree_node *node, list_node *data) {
    if (!node)
      return new_node(data);

    if (data->spec_cmp_phone_lt(*node->data))
      node->left = insert(node->left, data);
    else if (data->spec_cmp_phone_gt(*node->data))
      node->right = insert(node->right, data);
    else {
      delete data;
      if_action = nullptr;
      return node;
    }

    node->height = 1 + max_2(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);

    if (!balance || balance == -1 || balance == 1)
      return node;

    if (balance > 1 && data->spec_cmp_phone_lt(*node->left->data))
      return right_rotate(node);
    if (balance < -1 && data->spec_cmp_phone_gt(*node->right->data))
      return left_rotate(node);
    if (balance > 1 && data->spec_cmp_phone_gt(*node->left->data)) {
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }
    if (balance < -1 && data->spec_cmp_phone_lt(*node->right->data)) {
      node->right = right_rotate(node->right);
      return left_rotate(node);
    }

    return node;
  }

  list_node *insert(list_node *data) {
    if_action = nullptr;
    insert(root, data);
    return if_action;
  }

  tree_node *big_insert(tree_node *node, list_node *data) {
    if (!node)
      return new_node(data);

    if (data->big_spec_cmp_phone_lt(*node->data))
      node->left = big_insert(node->left, data);
    else if (data->big_spec_cmp_phone_gt(*node->data))
      node->right = big_insert(node->right, data);
    else {
      delete data;
      if_action = nullptr;
      return node;
    }

    node->height = 1 + max_2(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);

    if (!balance || balance == -1 || balance == 1)
      return node;

    if (balance > 1 && data->big_spec_cmp_phone_lt(*node->left->data))
      return right_rotate(node);
    if (balance < -1 && data->big_spec_cmp_phone_gt(*node->right->data))
      return left_rotate(node);
    if (balance > 1 && data->big_spec_cmp_phone_gt(*node->left->data)) {
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }
    if (balance < -1 && data->big_spec_cmp_phone_lt(*node->right->data)) {
      node->right = right_rotate(node->right);
      return left_rotate(node);
    }

    return node;
  }

  list_node *big_insert(list_node *data) {
    if_action = nullptr;
    big_insert(root, data);
    return if_action;
  }

  // ==================== DELETE TREE ====================

  void delete_tree(tree_node *new_root) {
    if (!new_root)
      return;
    if (new_root->left) {
      delete_tree(new_root->left);
      new_root->left = nullptr;
    }
    if (new_root->right) {
      delete_tree(new_root->right);
      new_root->right = nullptr;
    }
    delete new_root;
  }

  void delete_tree() {
    delete_tree(root);
    curr = root = nullptr;
    has_deleted = 0;
    if_action = nullptr;
  }

  tree() = default;
  ~tree() { delete_tree(); }

  tree_node *find_min(tree_node *node) {
    curr = node;
    while (curr->left != NULL)
      curr = curr->left;
    return curr;
  }

  // ==================== KILLER (DELETE BY NODE) ====================

  tree_node *killer(tree_node *node, list_node *data) {
    if (!node)
      return nullptr;

    if (data->spec_cmp_phone_lt(*node->data))
      node->left = killer(node->left, data);
    else if (data->spec_cmp_phone_gt(*node->data))
      node->right = killer(node->right, data);
    else {
      if (!node->left || !node->right) {
        tree_node *tmp = (node->left) ? (node->left) : (node->right);
        if (!tmp) {
          if (node == root)
            root = nullptr;
          delete node;
          has_deleted = 1;
          return nullptr;
        } else {
          if (node == root)
            root = tmp;
          delete node;
          has_deleted = 1;
          return tmp;
        }
      } else {
        tree_node *tmp = find_min(node->right);
        node->data = tmp->data;
        has_deleted = 1;
        node->right = killer(node->right, tmp->data);
      }
    }
    return balansirovka(node);
  }

  tree_node *killer(list_node *data) { return killer(root, data); }

  tree_node *big_killer(tree_node *node, list_node *data) {
    if (!node)
      return nullptr;

    if (data->big_spec_cmp_phone_lt(*node->data))
      node->left = big_killer(node->left, data);
    else if (data->big_spec_cmp_phone_gt(*node->data))
      node->right = big_killer(node->right, data);
    else {
      if (!node->left || !node->right) {
        tree_node *tmp = (node->left) ? (node->left) : (node->right);
        if (!tmp) {
          if (node == root)
            root = nullptr;
          delete node;
          has_deleted = 1;
          return nullptr;
        } else {
          if (node == root)
            root = tmp;
          delete node;
          has_deleted = 1;
          return tmp;
        }
      } else {
        tree_node *tmp = find_min(node->right);
        node->data = tmp->data;
        has_deleted = 1;
        node->right = big_killer(node->right, tmp->data);
      }
    }
    return balansirovka(node);
  }

  tree_node *big_killer(list_node *data) { return big_killer(root, data); }

  tree_node *killer_if_action(tree_node *node, list_node *data) {
    if (!node)
      return nullptr;

    if (data->spec_cmp_phone_lt(*node->data))
      node->left = killer_if_action(node->left, data);
    else if (data->spec_cmp_phone_gt(*node->data))
      node->right = killer_if_action(node->right, data);
    else {
      if (!node->left || !node->right) {
        tree_node *tmp = (node->left) ? (node->left) : (node->right);
        if (!tmp) {
          if (node == root)
            root = nullptr;
          if_action = node->data;
          delete node;
          has_deleted = 1;
          return nullptr;
        } else {
          if (node == root)
            root = tmp;
          if_action = node->data;
          delete node;
          has_deleted = 1;
          return tmp;
        }
      } else {
        tree_node *tmp = find_min(node->right);
        if_action = node->data;
        node->data = tmp->data;
        has_deleted = 1;
        node->right = killer_if_action(node->right, tmp->data);
      }
    }
    return balansirovka(node);
  }

  tree_node *killer_if_action() {
    if_action = nullptr;
    return killer_if_action(root, root->data);
  }

  // ==================== GENERIC SELECT OPERATIONS ====================
  // These replace ~100 repetitive select_cond_* functions

  // Full traversal select - for conditions that can't prune (NAME, etc.)
  void select_full_traversal(tree_node *node, command &cmd, CompareFunc cond) {
    if (!node)
      return;
    if ((cmd.*cond)(node->data))
      node->data->print();
    select_full_traversal(node->left, cmd, cond);
    select_full_traversal(node->right, cmd, cond);
  }

  void select_full_traversal(command &cmd, CompareFunc cond) {
    select_full_traversal(root, cmd, cond);
  }

  // Two conditions with AND - full traversal
  void select_two_cond_and(tree_node *node, command &cmd, CompareFunc cond1,
                           CompareFunc cond2) {
    if (!node)
      return;
    if ((cmd.*cond1)(node->data) && (cmd.*cond2)(node->data))
      node->data->print();
    select_two_cond_and(node->left, cmd, cond1, cond2);
    select_two_cond_and(node->right, cmd, cond1, cond2);
  }

  void select_two_cond_and(command &cmd, CompareFunc cond1, CompareFunc cond2) {
    select_two_cond_and(root, cmd, cond1, cond2);
  }

  // Two conditions with OR - full traversal
  void select_two_cond_or(tree_node *node, command &cmd, CompareFunc cond1,
                          CompareFunc cond2) {
    if (!node)
      return;
    if ((cmd.*cond1)(node->data) || (cmd.*cond2)(node->data))
      node->data->print();
    select_two_cond_or(node->left, cmd, cond1, cond2);
    select_two_cond_or(node->right, cmd, cond1, cond2);
  }

  void select_two_cond_or(command &cmd, CompareFunc cond1, CompareFunc cond2) {
    select_two_cond_or(root, cmd, cond1, cond2);
  }

  // ==================== OPTIMIZED PHONE SELECTS (O(log n) where possible)
  // ====================

  // PHONE_EQ: Exploit BST property - only go where phone could match
  void select_phone_eq(tree_node *node, command &cmd,
                       CompareFunc secondary = nullptr) {
    if (!node)
      return;

    if (cmd.phone == node->data->phone) {
      if (!secondary || (cmd.*secondary)(node->data))
        node->data->print();
      // Phone could appear in both subtrees (same phone, different name)
      select_phone_eq(node->left, cmd, secondary);
      select_phone_eq(node->right, cmd, secondary);
    } else if (cmd.phone < node->data->phone)
      select_phone_eq(node->left, cmd, secondary);
    else
      select_phone_eq(node->right, cmd, secondary);
  }

  void select_phone_eq(command &cmd, CompareFunc secondary = nullptr) {
    select_phone_eq(root, cmd, secondary);
  }

  // PHONE_NEQ: Must visit all nodes
  void select_phone_neq(tree_node *node, command &cmd,
                        CompareFunc secondary = nullptr) {
    if (!node)
      return;
    if (cmd.phone != node->data->phone) {
      if (!secondary || (cmd.*secondary)(node->data))
        node->data->print();
    }
    select_phone_neq(node->left, cmd, secondary);
    select_phone_neq(node->right, cmd, secondary);
  }

  void select_phone_neq(command &cmd, CompareFunc secondary = nullptr) {
    select_phone_neq(root, cmd, secondary);
  }

  // PHONE_LT: Only need to check nodes with phone < cmd.phone
  void select_phone_lt(tree_node *node, command &cmd,
                       CompareFunc secondary = nullptr) {
    if (!node)
      return;

    if (node->data->phone < cmd.phone) {
      if (!secondary || (cmd.*secondary)(node->data))
        node->data->print();
      select_phone_lt(node->left, cmd, secondary);
      select_phone_lt(node->right, cmd, secondary);
    } else {
      // Current >= cmd.phone, only left subtree can have smaller
      select_phone_lt(node->left, cmd, secondary);
    }
  }

  void select_phone_lt(command &cmd, CompareFunc secondary = nullptr) {
    select_phone_lt(root, cmd, secondary);
  }

  // PHONE_LE: phone <= cmd.phone
  void select_phone_le(tree_node *node, command &cmd,
                       CompareFunc secondary = nullptr) {
    if (!node)
      return;

    if (node->data->phone <= cmd.phone) {
      if (!secondary || (cmd.*secondary)(node->data))
        node->data->print();
      select_phone_le(node->left, cmd, secondary);
      select_phone_le(node->right, cmd, secondary);
    } else {
      select_phone_le(node->left, cmd, secondary);
    }
  }

  void select_phone_le(command &cmd, CompareFunc secondary = nullptr) {
    select_phone_le(root, cmd, secondary);
  }

  // PHONE_GT: Only check nodes with phone > cmd.phone
  void select_phone_gt(tree_node *node, command &cmd,
                       CompareFunc secondary = nullptr) {
    if (!node)
      return;

    if (node->data->phone > cmd.phone) {
      if (!secondary || (cmd.*secondary)(node->data))
        node->data->print();
      select_phone_gt(node->left, cmd, secondary);
      select_phone_gt(node->right, cmd, secondary);
    } else {
      // Current <= cmd.phone, only right subtree can have larger
      select_phone_gt(node->right, cmd, secondary);
    }
  }

  void select_phone_gt(command &cmd, CompareFunc secondary = nullptr) {
    select_phone_gt(root, cmd, secondary);
  }

  // PHONE_GE: phone >= cmd.phone
  void select_phone_ge(tree_node *node, command &cmd,
                       CompareFunc secondary = nullptr) {
    if (!node)
      return;

    if (node->data->phone >= cmd.phone) {
      if (!secondary || (cmd.*secondary)(node->data))
        node->data->print();
      select_phone_ge(node->left, cmd, secondary);
      select_phone_ge(node->right, cmd, secondary);
    } else {
      select_phone_ge(node->right, cmd, secondary);
    }
  }

  void select_phone_ge(command &cmd, CompareFunc secondary = nullptr) {
    select_phone_ge(root, cmd, secondary);
  }

  // ==================== GENERIC DELETE WITH CONDITIONS ====================

  // Delete with phone EQ condition
  tree_node *delete_phone_eq(tree_node *node, command &cmd,
                             CompareFunc secondary = nullptr) {
    if (!node)
      return nullptr;

    if (cmd.phone < node->data->phone)
      node->left = delete_phone_eq(node->left, cmd, secondary);
    else if (cmd.phone > node->data->phone)
      node->right = delete_phone_eq(node->right, cmd, secondary);
    else {
      // Phone matches - check secondary condition if present
      if (secondary && !(cmd.*secondary)(node->data)) {
        // Secondary condition doesn't match, check both subtrees
        node->left = delete_phone_eq(node->left, cmd, secondary);
        node->right = delete_phone_eq(node->right, cmd, secondary);
        return balansirovka(node);
      }

      // Delete this node
      if (!node->left || !node->right) {
        tree_node *tmp = (node->left) ? (node->left) : (node->right);
        if (!tmp) {
          if (node == root)
            root = nullptr;
          if_action = node->data;
          delete node;
          has_deleted = 1;
          return nullptr;
        } else {
          if (node == root)
            root = tmp;
          if_action = node->data;
          delete node;
          has_deleted = 1;
          return tmp;
        }
      } else {
        tree_node *tmp = find_min(node->right);
        if_action = node->data;
        node->data = tmp->data;
        has_deleted = 1;
        node->right = killer(node->right, tmp->data);
      }
    }
    return balansirovka(node);
  }

  tree_node *delete_phone_eq(command &cmd, CompareFunc secondary = nullptr) {
    if_action = nullptr;
    return delete_phone_eq(root, cmd, secondary);
  }

  // Delete with phone EQ (big version for global index)
  tree_node *big_delete_phone_eq(tree_node *node, command &cmd) {
    if (!node)
      return nullptr;

    if (cmd.phone < node->data->phone)
      node->left = big_delete_phone_eq(node->left, cmd);
    else if (cmd.phone > node->data->phone)
      node->right = big_delete_phone_eq(node->right, cmd);
    else {
      if (!node->left || !node->right) {
        tree_node *tmp = (node->left) ? (node->left) : (node->right);
        if (!tmp) {
          if (node == root)
            root = nullptr;
          if_action = node->data;
          delete node;
          has_deleted = 1;
          return nullptr;
        } else {
          if (node == root)
            root = tmp;
          if_action = node->data;
          delete node;
          has_deleted = 1;
          return tmp;
        }
      } else {
        tree_node *tmp = find_min(node->right);
        if_action = node->data;
        node->data = tmp->data;
        has_deleted = 1;
        node->right = big_killer(node->right, tmp->data);
      }
    }
    return balansirovka(node);
  }

  tree_node *big_delete_phone_eq(command &cmd) {
    if_action = nullptr;
    return big_delete_phone_eq(root, cmd);
  }

  // Generic delete with full traversal (for non-indexed conditions)
  void delete_full_traversal(tree_node *node, command &cmd, CompareFunc cond) {
    if (!node)
      return;

    if ((cmd.*cond)(node->data)) {
      if_action = node->data;
      // Mark for deletion, but we need to handle this carefully
      // The caller should loop until no more matches
    }
    delete_full_traversal(node->left, cmd, cond);
    delete_full_traversal(node->right, cmd, cond);
  }

  // ==================== COMPATIBILITY WRAPPERS ====================
  // These provide the old interface for existing code

  // NAME conditions - full traversal required
  void select_cond_NAME_EQ(command &cmd) {
    select_full_traversal(cmd, &command::cmp_NAME_EQ);
  }

  void select_cond_NAME_LIKE(command &cmd) {
    select_full_traversal(cmd, &command::cmp_NAME_LIKE);
  }

  // PHONE conditions - optimized
  void select_cond_PHONE_EQ(command &cmd) { select_phone_eq(cmd); }

  void select_cond_PHONE_NEQ(command &cmd) { select_phone_neq(cmd); }

  void select_cond_PHONE_LT(command &cmd) { select_phone_lt(cmd); }

  void select_cond_PHONE_LE(command &cmd) { select_phone_le(cmd); }

  void select_cond_PHONE_GT(command &cmd) { select_phone_gt(cmd); }

  void select_cond_PHONE_GE(command &cmd) { select_phone_ge(cmd); }

  // PHONE + NAME combinations
  void select_cond_PHONE_EQ_AND_NAME_EQ(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_EQ);
  }

  void select_cond_PHONE_EQ_AND_NAME_NEQ(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_NEQ);
  }

  void select_cond_PHONE_EQ_AND_NAME_LT(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_LT);
  }

  void select_cond_PHONE_EQ_AND_NAME_GT(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_GT);
  }

  void select_cond_PHONE_EQ_AND_NAME_LE(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_LE);
  }

  void select_cond_PHONE_EQ_AND_NAME_GE(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_GE);
  }

  void select_cond_PHONE_EQ_AND_NAME_LIKE(command &cmd) {
    select_phone_eq(cmd, &command::cmp_NAME_LIKE);
  }

  void select_cond_PHONE_LT_AND_NAME_EQ(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_EQ);
  }

  void select_cond_PHONE_LT_AND_NAME_NEQ(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_NEQ);
  }

  void select_cond_PHONE_LT_AND_NAME_LT(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_LT);
  }

  void select_cond_PHONE_LT_AND_NAME_GT(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_GT);
  }

  void select_cond_PHONE_LT_AND_NAME_LE(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_LE);
  }

  void select_cond_PHONE_LT_AND_NAME_GE(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_GE);
  }

  void select_cond_PHONE_LT_AND_NAME_LIKE(command &cmd) {
    select_phone_lt(cmd, &command::cmp_NAME_LIKE);
  }

  void select_cond_PHONE_GT_AND_NAME_EQ(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_EQ);
  }

  void select_cond_PHONE_GT_AND_NAME_NEQ(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_NEQ);
  }

  void select_cond_PHONE_GT_AND_NAME_LT(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_LT);
  }

  void select_cond_PHONE_GT_AND_NAME_GT(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_GT);
  }

  void select_cond_PHONE_GT_AND_NAME_LE(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_LE);
  }

  void select_cond_PHONE_GT_AND_NAME_GE(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_GE);
  }

  void select_cond_PHONE_GT_AND_NAME_LIKE(command &cmd) {
    select_phone_gt(cmd, &command::cmp_NAME_LIKE);
  }

  void select_cond_PHONE_LE_AND_NAME_EQ(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_EQ);
  }

  void select_cond_PHONE_LE_AND_NAME_NEQ(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_NEQ);
  }

  void select_cond_PHONE_LE_AND_NAME_LT(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_LT);
  }

  void select_cond_PHONE_LE_AND_NAME_GT(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_GT);
  }

  void select_cond_PHONE_LE_AND_NAME_LE(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_LE);
  }

  void select_cond_PHONE_LE_AND_NAME_GE(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_GE);
  }

  void select_cond_PHONE_LE_AND_NAME_LIKE(command &cmd) {
    select_phone_le(cmd, &command::cmp_NAME_LIKE);
  }

  void select_cond_PHONE_GE_AND_NAME_EQ(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_EQ);
  }

  void select_cond_PHONE_GE_AND_NAME_NEQ(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_NEQ);
  }

  void select_cond_PHONE_GE_AND_NAME_LT(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_LT);
  }

  void select_cond_PHONE_GE_AND_NAME_GT(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_GT);
  }

  void select_cond_PHONE_GE_AND_NAME_LE(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_LE);
  }

  void select_cond_PHONE_GE_AND_NAME_GE(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_GE);
  }

  void select_cond_PHONE_GE_AND_NAME_LIKE(command &cmd) {
    select_phone_ge(cmd, &command::cmp_NAME_LIKE);
  }

  // PHONE + GROUP combinations
  void select_cond_PHONE_EQ_AND_GROUP_EQ(command &cmd) {
    select_phone_eq(cmd, &command::cmp_GROUP_EQ);
  }

  void select_cond_PHONE_EQ_AND_GROUP_NEQ(command &cmd) {
    select_phone_eq(cmd, &command::cmp_GROUP_NEQ);
  }

  void select_cond_PHONE_EQ_AND_GROUP_LT(command &cmd) {
    select_phone_eq(cmd, &command::cmp_GROUP_LT);
  }

  void select_cond_PHONE_EQ_AND_GROUP_GT(command &cmd) {
    select_phone_eq(cmd, &command::cmp_GROUP_GT);
  }

  void select_cond_PHONE_EQ_AND_GROUP_LE(command &cmd) {
    select_phone_eq(cmd, &command::cmp_GROUP_LE);
  }

  void select_cond_PHONE_EQ_AND_GROUP_GE(command &cmd) {
    select_phone_eq(cmd, &command::cmp_GROUP_GE);
  }

  void select_cond_PHONE_LT_AND_GROUP_EQ(command &cmd) {
    select_phone_lt(cmd, &command::cmp_GROUP_EQ);
  }

  void select_cond_PHONE_LT_AND_GROUP_NEQ(command &cmd) {
    select_phone_lt(cmd, &command::cmp_GROUP_NEQ);
  }

  void select_cond_PHONE_LT_AND_GROUP_LT(command &cmd) {
    select_phone_lt(cmd, &command::cmp_GROUP_LT);
  }

  void select_cond_PHONE_LT_AND_GROUP_GT(command &cmd) {
    select_phone_lt(cmd, &command::cmp_GROUP_GT);
  }

  void select_cond_PHONE_LT_AND_GROUP_LE(command &cmd) {
    select_phone_lt(cmd, &command::cmp_GROUP_LE);
  }

  void select_cond_PHONE_LT_AND_GROUP_GE(command &cmd) {
    select_phone_lt(cmd, &command::cmp_GROUP_GE);
  }

  void select_cond_PHONE_GT_AND_GROUP_EQ(command &cmd) {
    select_phone_gt(cmd, &command::cmp_GROUP_EQ);
  }

  void select_cond_PHONE_GT_AND_GROUP_NEQ(command &cmd) {
    select_phone_gt(cmd, &command::cmp_GROUP_NEQ);
  }

  void select_cond_PHONE_GT_AND_GROUP_LT(command &cmd) {
    select_phone_gt(cmd, &command::cmp_GROUP_LT);
  }

  void select_cond_PHONE_GT_AND_GROUP_GT(command &cmd) {
    select_phone_gt(cmd, &command::cmp_GROUP_GT);
  }

  void select_cond_PHONE_GT_AND_GROUP_LE(command &cmd) {
    select_phone_gt(cmd, &command::cmp_GROUP_LE);
  }

  void select_cond_PHONE_GT_AND_GROUP_GE(command &cmd) {
    select_phone_gt(cmd, &command::cmp_GROUP_GE);
  }

  void select_cond_PHONE_LE_AND_GROUP_EQ(command &cmd) {
    select_phone_le(cmd, &command::cmp_GROUP_EQ);
  }

  void select_cond_PHONE_LE_AND_GROUP_NEQ(command &cmd) {
    select_phone_le(cmd, &command::cmp_GROUP_NEQ);
  }

  void select_cond_PHONE_LE_AND_GROUP_LT(command &cmd) {
    select_phone_le(cmd, &command::cmp_GROUP_LT);
  }

  void select_cond_PHONE_LE_AND_GROUP_GT(command &cmd) {
    select_phone_le(cmd, &command::cmp_GROUP_GT);
  }

  void select_cond_PHONE_LE_AND_GROUP_LE(command &cmd) {
    select_phone_le(cmd, &command::cmp_GROUP_LE);
  }

  void select_cond_PHONE_LE_AND_GROUP_GE(command &cmd) {
    select_phone_le(cmd, &command::cmp_GROUP_GE);
  }

  void select_cond_PHONE_GE_AND_GROUP_EQ(command &cmd) {
    select_phone_ge(cmd, &command::cmp_GROUP_EQ);
  }

  void select_cond_PHONE_GE_AND_GROUP_NEQ(command &cmd) {
    select_phone_ge(cmd, &command::cmp_GROUP_NEQ);
  }

  void select_cond_PHONE_GE_AND_GROUP_LT(command &cmd) {
    select_phone_ge(cmd, &command::cmp_GROUP_LT);
  }

  void select_cond_PHONE_GE_AND_GROUP_GT(command &cmd) {
    select_phone_ge(cmd, &command::cmp_GROUP_GT);
  }

  void select_cond_PHONE_GE_AND_GROUP_LE(command &cmd) {
    select_phone_ge(cmd, &command::cmp_GROUP_LE);
  }

  void select_cond_PHONE_GE_AND_GROUP_GE(command &cmd) {
    select_phone_ge(cmd, &command::cmp_GROUP_GE);
  }

  // Delete wrappers
  tree_node *delete_node_phone_EQ(command &cmd) { return delete_phone_eq(cmd); }

  tree_node *big_delete_node_phone_EQ(command &cmd) {
    return big_delete_phone_eq(cmd);
  }

  void delete_node_phone_EQ_and_name_EQ(command &cmd) {
    delete_phone_eq(cmd, &command::cmp_NAME_EQ);
  }

  // ==================== PRINT ====================

#define MAX_DEPTH 400

  void print_tree(tree_node *node, int depth) {
    if (depth > MAX_DEPTH)
      return;

    if (node) {
      node->data->print(depth);
      schet++;
      print_tree(node->left, depth + 1);
      print_tree(node->right, depth + 1);
    }
  }

  void print_tree() {
    schet = 0;
    if (!root) {
      printf("Empty tree\n\n");
      return;
    }

    print_tree(root, 0);
    printf("chet = %d\n", schet);
    if (schet == 0)
      root->data->print();
    schet = 0;
  }

  void print() { print_tree(); }

#undef MAX_DEPTH

  // ==================== ACCESSORS ====================

  void add_right(tree_node *right) { curr->set_right(right); }
  void add_left(tree_node *left) { curr->set_left(left); }
  void add_root(tree_node *new_root) { root = new_root; }

  tree_node *get_left() const { return curr->left; }
  tree_node *get_right() const { return curr->right; }
  tree_node *get_root() const { return root; }
};

#endif // AVL_TREE_H
