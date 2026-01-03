#include "parser.h"
#include <cstring>
#define MAX_PRINT 15
void record::destroy() {
  name = 0;
  phone = group = -1;
}

record::~record() { destroy(); }

int record::init(const char *n, int ph, int gr) {
  phone = ph;
  group = gr;
  if (n) {
    name = std::unique_ptr<char[]>(new char[strlen(n) + 1]);
    if (!name.get())
      return -1;
    strcpy(name.get(), n);
  } else
    name = 0;
  return 0;
}

int record::init_name(const char *n) {
  if (n) {
    const char *start = n;
    const char *end = n + strlen(n) - 1;

    // Strip surrounding quotes if present
    if (((*start == '\'' && *end == '\'') || (*start == '"' && *end == '"')) &&
        start < end) {
      start++;
      end--;
    }

    int len = end - start + 1;
    name = std::unique_ptr<char[]>(new char[len + 1]);
    if (!name.get())
      return -1;
    strncpy(name.get(), start, len);
    name.get()[len] = '\0';
  } else
    name = 0;
  return 0;
}

int record::read(FILE *fp) {
  char buf[LEN];
  destroy();
  if (fscanf(fp, "%s%d%d", buf, &phone, &group) != 3) {
    if (feof(fp))
      return 1;
    return -1;
  }
  if (init(buf, phone, group))
    return -2;
  return 0;
}

// comparators for tree

int record::spec_cmp_phone_gt(
    const record &rhs) // 0 if this->phone > rhs->phone
{                      // return 1
  int ret;
  if ((ret = phone - rhs.phone) > 0)
    return 1;
  if (ret < 0)
    return 0;
  // ret == 0
  /*if (!(get_name()) && !rhs.get_name()) //FIND ME
      return 0;                         //DELETE CASES FOR NAME == NULLPTR
  if (!get_name()) return 0;                  //spoiler: there's so such stuff
  if (!rhs.get_name()) return 1; */             //and maybe this is free speedup

  ret = strcmp(get_name(), rhs.get_name());
  if (ret > 0)
    return 1;
  return 0;
}

int record::spec_cmp_phone_lt(
    const record &rhs) // 0 if this->phone < rhs->phone
{                      // return 1
  int ret;
  if ((ret = phone - rhs.phone) > 0)
    return 0;
  if (ret < 0)
    return 1;
  // ret == 0
  /*if (!(get_name()) && !rhs.get_name()) //FIND ME
      return 0;                         //DELETE CASES FOR NAME == NULLPTR
  if (!get_name()) return 1;                  //spoiler: there's so such stuff
  if (!rhs.get_name()) return 0;  */           //and maybe this is free speedup

  ret = strcmp(get_name(), rhs.get_name());
  if (ret < 0)
    return 1;
  return 0;
}

int record::big_spec_cmp_phone_gt(
    const record &rhs) // 0 if this->phone > rhs->phone
{                      // return 1
  int ret;
  if ((ret = phone - rhs.phone) > 0)
    return 1;
  if (ret < 0)
    return 0;
  // ret == 0
  if ((ret = group - rhs.group) > 0)
    return 1;
  if (ret < 0)
    return 0;
  // ret == 0 once again
  /*if (!(get_name()) && !rhs.get_name()) //FIND ME
      return 0;                         //DELETE CASES FOR NAME == NULLPTR
  if (!get_name()) return 0;                  //spoiler: there's so such stuff
  if (!rhs.get_name()) return 1;    */          //and maybe this is free speedup

  ret = strcmp(get_name(), rhs.get_name());
  if (ret > 0)
    return 1;
  return 0;
}

int record::big_spec_cmp_phone_lt(
    const record &rhs) // 0 if this->phone < rhs->phone
{                      // return 1
  int ret;
  if ((ret = phone - rhs.phone) > 0)
    return 0;
  if (ret < 0)
    return 1;
  // ret == 0
  if ((ret = group - rhs.group) > 0)
    return 0;
  if (ret < 0)
    return 1;
  // ret == 0 once more
  /*if (!(get_name()) && !rhs.get_name()) //FIND ME
      return 0;                         //DELETE CASES FOR NAME == NULLPTR
  if (!get_name()) return 1;                  //spoiler: there's so such stuff
  if (!rhs.get_name()) return 0;  */           //and maybe this is free speedup

  ret = strcmp(get_name(), rhs.get_name());
  if (ret < 0)
    return 1;
  return 0;
}

int record::operator==(const record &rhs) {
  if (group != rhs.group)
    return 0;
  if (phone != rhs.phone)
    return 0;
  if (!(get_name()) && !(rhs.get_name()))
    return 1; // FIND ME
  if (!(get_name()) || !(rhs.get_name()))
    return 0;
  if (strcmp(get_name(), rhs.get_name()))
    return 0;
  return 1;
}

// TODO ALL THE OTHER STUFF

//-----command-----------------

// basic comparaters
int command::cmp_NAME_EQ(record *current) {
  if (!strcmp(current->get_name(), get_name()))
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_NAME_NEQ(record *current) {
  if (strcmp(current->get_name(), get_name()))
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_NAME_LT(record *current) {
  if (strcmp(current->get_name(), get_name()) < 0)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_NAME_GT(record *current) {
  if (strcmp(current->get_name(), get_name()) > 0)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_NAME_LE(record *current) {
  if (strcmp(current->get_name(), get_name()) <= 0)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_NAME_GE(record *current) {
  if (strcmp(current->get_name(), get_name()) >= 0)
    return 1; // satisfies
  return 0;   // not
}

/*static char **table;

void command::init_table()
{
    table = new char*[2];
    table[0] = new char[LEN + 1];
    table[1] = new char[LEN + 1];
}

void command::delete_table()
{
    delete[] table[0]; delete []table[1]; delete []table;
}*/

int command::cmp_NAME_LIKE(record *current) //
{
  const char *s1 = get_name(), *s2 = current->get_name();

  char table[2][LEN + 1];
  int i = 0, j = 0;
  bool if_first = 1;

  for (; s1[i]; i++) {
    if (s1[i] == '%')
      break;
  }

  if (!s1[i]) {
    for (i = 0; s1[i] && s2[i]; i++) {
      if (s1[i] != '_' && s1[i] != s2[i])
        break;
    }
    return !s1[i] && !s2[i];
  }

  if (!s1[0])
    return !s2[0];

  table[0][0] = 1;
  for (i = 0; s1[i] == '%'; i++)
    table[0][i + 1] = 1;
  for (; s1[i]; i++)
    table[0][i + 1] = 0;

  for (i = 0; s2[i]; i++) {
    table[if_first][0] = 0;
    for (j = 0; s1[j]; j++) {
      if (s1[j] == '%') {
        table[if_first][j + 1] =
            table[1 - if_first][j + 1] || table[if_first][j];
      } else if (s1[j] == '_' || s1[j] == s2[i]) {
        table[if_first][j + 1] = table[1 - if_first][j];
      } else
        table[if_first][j + 1] = 0;
    }

    if_first = 1 - if_first;
  }

  return table[1 - if_first][j];
}
//#endif

//----
int command::cmp_PHONE_GT(record *current) {
  if (current->phone > phone)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_PHONE_LT(record *current) {
  if (current->phone < phone)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_PHONE_GE(record *current) {
  if (current->phone >= phone)
    return 1; // satisfies
  return 0;   // not
}
int command::cmp_PHONE_LE(record *current) {
  if (current->phone <= phone)
    return 1; // satisfies
  return 0;   // not
}
int command::cmp_PHONE_EQ(record *current) {
  if (current->phone == phone)
    return 1; // satisfies
  return 0;   // not
}
int command::cmp_PHONE_NEQ(record *current) {
  if (current->phone != phone)
    return 1; // satisfies
  return 0;   // not
}
//-------------

int command::cmp_GROUP_EQ(record *current) {
  if (current->group == group)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_GROUP_NEQ(record *current) {
  if (current->group != group)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_GROUP_LT(record *current) {
  if (current->group < group)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_GROUP_GT(record *current) {
  if (current->group > group)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_GROUP_LE(record *current) {
  if (current->group <= group)
    return 1; // satisfies
  return 0;   // not
}

int command::cmp_GROUP_GE(record *current) {
  if (current->group >= group)
    return 1; // satisfies
  return 0;   // not
}

//------------ PARSER -------------------------------------------------------
int command::parser(parse &p) {
  if (p.perstr > 0) {
    if (!strcmp(p.words[0], "select")) // p.perstr carries???
    {
      type = SELECT;
      if (p.perstr != 2 && p.perstr != 6 && p.perstr != 10) {
        printf("Bad input 18\n");
        return -1;
      }

      if (!strcmp(p.words[1], "*")) {
        if (p.perstr == 2) {
          return 0;
        }

        if (p.perstr > 2 && !strcmp(p.words[2], "where")) {
          if (p.perstr <= 3) {
            printf("Bad input 19\n");
            return -1;
          }

          if (!strcmp(p.words[3], "name")) {
            if (p.perstr <= 4) {
              printf("Bad input 12\n");
              return -1;
            }

            if (!strcmp(p.words[4], "=")) {
              c_name = EQ;
            } else if (!strcmp(p.words[4], "<>")) {
              c_name = NEQ;
            } else if (!strcmp(p.words[4], "<")) {
              c_name = LT;
            } else if (!strcmp(p.words[4], ">")) {
              c_name = GT;
            } else if (!strcmp(p.words[4], "<=")) {
              c_name = LE;
            } else if (!strcmp(p.words[4], ">=")) {
              c_name = GE;
            } else if (!strcmp(p.words[4], "like")) {
              c_name = LIKE;
            }

            if (c_name == COND_NONE) {
              printf("Bad input 13\n");
              return -1;
            }

            if (p.perstr > 5) {
              init_name(p.words[5]);
              if (p.perstr <= 6)
                return 0;

              //----6------------
              if (!strcmp(p.words[6], "and")) {
                op = AND;
              } else if (!strcmp(p.words[6], "or")) {
                op = OR;
              }

              if (op == OP_NONE) {
                printf("Bad input 14\n");
                return -1;
              }

              //------7--------
              if (p.perstr <= 7) {
                printf("Bad input 5\n");
                return -1;
              }

              //-----phone--------
              if (!strcmp(p.words[7], "phone")) {
                if (p.perstr <= 8) {
                  printf("Bad input 4\n");
                  return -1;
                }

                if (!strcmp(p.words[8], "=")) {
                  c_phone = EQ;
                } else if (!strcmp(p.words[8], "<>")) {
                  c_phone = NEQ;
                } else if (!strcmp(p.words[8], "<")) {
                  c_phone = LT;
                } else if (!strcmp(p.words[8], ">")) {
                  c_phone = GT;
                } else if (!strcmp(p.words[8], "<=")) {
                  c_phone = LE;
                } else if (!strcmp(p.words[8], ">=")) {
                  c_phone = GE;
                } else if (!strcmp(p.words[8], "like")) {
                  c_phone = LIKE;
                }

                if (c_phone == COND_NONE) {
                  printf("Bad input 15\n");
                  return -1;
                }

                if (p.perstr > 9) {
                  if (!strcmp(p.words[9], "0")) {
                    phone = 0;
                    return 0;
                  } else {
                    if (atoi(p.words[9])) {
                      phone = atoi(p.words[9]);
                      return 0;
                    } else {
                      printf("Bad input 16\n");
                      return -1;
                    }
                  }
                }
                // else
                printf("Bad input 17\n");
                return -1;
              }
              //----group----------
              if (!strcmp(p.words[7], "group")) {
                if (p.perstr <= 8) {
                  printf("Bad input 6\n");
                  return -1;
                }

                if (!strcmp(p.words[8], "=")) {
                  c_group = EQ;
                } else if (!strcmp(p.words[8], "<>")) {
                  c_group = NEQ;
                } else if (!strcmp(p.words[8], "<")) {
                  c_group = LT;
                } else if (!strcmp(p.words[8], ">")) {
                  c_group = GT;
                } else if (!strcmp(p.words[8], "<=")) {
                  c_group = LE;
                } else if (!strcmp(p.words[8], ">=")) {
                  c_group = GE;
                } else if (!strcmp(p.words[8], "like")) {
                  c_group = LIKE;
                }

                if (c_group == COND_NONE) {
                  printf("Bad input 7\n");
                  return -1;
                }

                if (p.perstr > 9) {
                  if (!strcmp(p.words[9], "0")) {
                    group = 0;
                    return 0;
                  } else if (atoi(p.words[9])) {
                    group = atoi(p.words[9]);
                    return 0;
                  } else {
                    printf("Bad input 8\n");
                    return -1;
                  }
                }
                // else
                printf("Bad input 9\n");
                return -1;
              }
              // else
              printf("Bad input 10\n");
              return -1;

            } // p.perstr <= 5
              // else
            printf("Bad input 20\n");
            return -1;
          } // not name after where

          else if (!strcmp(p.words[3], "phone")) {
            if (p.perstr <= 4) {
              printf("Bad input 21\n");
              return -1;
            }

            if (!strcmp(p.words[4], "=")) {
              c_phone = EQ;
            } else if (!strcmp(p.words[4], "<>")) {
              c_phone = NEQ;
            } else if (!strcmp(p.words[4], "<")) {
              c_phone = LT;
            } else if (!strcmp(p.words[4], ">")) {
              c_phone = GT;
            } else if (!strcmp(p.words[4], "<=")) {
              c_phone = LE;
            } else if (!strcmp(p.words[4], ">=")) {
              c_phone = GE;
            } else if (!strcmp(p.words[4], "like")) {
              c_phone = LIKE;
            }

            if (c_phone == COND_NONE) {
              printf("Bad input 22\n");
              return -1;
            }

            if (p.perstr > 5) {
              // phone = p.words[5];
              if (!strcmp(p.words[5], "0")) {
                phone = 0;
              } else if (atoi(p.words[5])) {
                phone = atoi(p.words[5]);
              } else {
                printf("Bad input 23\n");
                return -1;
              }

              if (p.perstr <= 6)
                return 0;

              //----6------------
              if (!strcmp(p.words[6], "and")) {
                op = AND;
              } else if (!strcmp(p.words[6], "or")) {
                op = OR;
              }

              if (op == OP_NONE) {
                printf("Bad input 24\n");
                return -1;
              }

              //------7--------
              if (p.perstr <= 7) {
                printf("Bad input 25\n");
                return -1;
              }

              //-----name--------
              if (!strcmp(p.words[7], "name")) {
                if (p.perstr <= 8) {
                  printf("Bad input 26\n");
                  return -1;
                }

                if (!strcmp(p.words[8], "=")) {
                  c_name = EQ;
                } else if (!strcmp(p.words[8], "<>")) {
                  c_name = NEQ;
                } else if (!strcmp(p.words[8], "<")) {
                  c_name = LT;
                } else if (!strcmp(p.words[8], ">")) {
                  c_name = GT;
                } else if (!strcmp(p.words[8], "<=")) {
                  c_name = LE;
                } else if (!strcmp(p.words[8], ">=")) {
                  c_name = GE;
                } else if (!strcmp(p.words[8], "like")) {
                  c_name = LIKE;
                }

                if (c_name == COND_NONE) {
                  printf("Bad input 27\n");
                  return -1;
                }

                if (p.perstr > 9) {
                  init_name(p.words[9]);
                  return 0;
                }
                // else
                printf("Bad input 28\n");
                return -1;
              }
              //----group----------
              if (!strcmp(p.words[7], "group")) {
                if (p.perstr <= 8) {
                  printf("Bad input 29\n");
                  return -1;
                }

                if (!strcmp(p.words[8], "=")) {
                  c_group = EQ;
                } else if (!strcmp(p.words[8], "<>")) {
                  c_group = NEQ;
                } else if (!strcmp(p.words[8], "<")) {
                  c_group = LT;
                } else if (!strcmp(p.words[8], ">")) {
                  c_group = GT;
                } else if (!strcmp(p.words[8], "<=")) {
                  c_group = LE;
                } else if (!strcmp(p.words[8], ">=")) {
                  c_group = GE;
                } else if (!strcmp(p.words[8], "like")) {
                  c_group = LIKE;
                }

                if (c_group == COND_NONE) {
                  printf("Bad input 30\n");
                  return -1;
                }

                if (p.perstr > 9) {
                  if (!strcmp(p.words[9], "0")) {
                    group = 0;
                    return 0;
                  } else if (atoi(p.words[9])) {
                    group = atoi(p.words[9]);
                    return 0;
                  } else {
                    printf("Bad input 31\n");
                    return -1;
                  }
                }
                // else
                printf("Bad input 32\n");
                return -1;
              }
              // else
              printf("Bad input 33\n");
              return -1;

            } // p.perstr <= 5
              // else
            printf("Bad input 20\n");
            return -1;
          } // not name  nor phone after where

          else if (!strcmp(p.words[3], "group")) {
            if (p.perstr <= 4) {
              printf("Bad input 35\n");
              return -1;
            }

            if (!strcmp(p.words[4], "=")) {
              c_group = EQ;
            } else if (!strcmp(p.words[4], "<>")) {
              c_group = NEQ;
            } else if (!strcmp(p.words[4], "<")) {
              c_group = LT;
            } else if (!strcmp(p.words[4], ">")) {
              c_group = GT;
            } else if (!strcmp(p.words[4], "<=")) {
              c_group = LE;
            } else if (!strcmp(p.words[4], ">=")) {
              c_group = GE;
            } else if (!strcmp(p.words[4], "like")) {
              c_group = LIKE;
            }

            if (c_group == COND_NONE) {
              printf("Bad input 36\n");
              return -1;
            }

            if (p.perstr > 5) {
              // group = p.words[5];
              if (!strcmp(p.words[5], "0")) {
                group = 0;
              } else if (atoi(p.words[5])) {
                group = atoi(p.words[5]);
              } else {
                printf("Bad input 37\n");
                return -1;
              }

              if (p.perstr <= 6)
                return 0;

              //----6------------
              if (!strcmp(p.words[6], "and")) {
                op = AND;
              } else if (!strcmp(p.words[6], "or")) {
                op = OR;
              }

              if (op == OP_NONE) {
                printf("Bad input 38\n");
                return -1;
              }

              //------7--------
              if (p.perstr <= 7) {
                printf("Bad input 39\n");
                return -1;
              }

              //-----name--------
              if (!strcmp(p.words[7], "name")) {
                if (p.perstr <= 8) {
                  printf("Bad input 40\n");
                  return -1;
                }

                if (!strcmp(p.words[8], "=")) {
                  c_name = EQ;
                } else if (!strcmp(p.words[8], "<>")) {
                  c_name = NEQ;
                } else if (!strcmp(p.words[8], "<")) {
                  c_name = LT;
                } else if (!strcmp(p.words[8], ">")) {
                  c_name = GT;
                } else if (!strcmp(p.words[8], "<=")) {
                  c_name = LE;
                } else if (!strcmp(p.words[8], ">=")) {
                  c_name = GE;
                } else if (!strcmp(p.words[8], "like")) {
                  c_name = LIKE;
                }

                if (c_name == COND_NONE) {
                  printf("Bad input 40\n");
                  return -1;
                }

                if (p.perstr > 9) {
                  init_name(p.words[9]);
                  return 0;
                }
                // else
                printf("Bad input 41\n");
                return -1;
              }
              //----phone----------
              if (!strcmp(p.words[7], "phone")) {
                if (p.perstr <= 8) {
                  printf("Bad input 42\n");
                  return -1;
                }

                if (!strcmp(p.words[8], "=")) {
                  c_phone = EQ;
                } else if (!strcmp(p.words[8], "<>")) {
                  c_phone = NEQ;
                } else if (!strcmp(p.words[8], "<")) {
                  c_phone = LT;
                } else if (!strcmp(p.words[8], ">")) {
                  c_phone = GT;
                } else if (!strcmp(p.words[8], "<=")) {
                  c_phone = LE;
                } else if (!strcmp(p.words[8], ">=")) {
                  c_phone = GE;
                } else if (!strcmp(p.words[8], "like")) {
                  c_phone = LIKE;
                }

                if (c_phone == COND_NONE) {
                  printf("Bad input 43\n");
                  return -1;
                }

                if (p.perstr > 9) {
                  if (!strcmp(p.words[9], "0")) {
                    phone = 0;
                    return 0;
                  } else if (atoi(p.words[9])) {
                    phone = atoi(p.words[9]);
                    return 0;
                  } else {
                    printf("Bad input 44\n");
                    return -1;
                  }
                }
                // else
                printf("Bad input 45\n");
                return -1;
              }
              // else
              printf("Bad input 46\n");
              return -1;

            } // p.perstr <= 5
              // else
            printf("Bad input 20\n");
            return -1;
          } // not name  nor phone nor group after where
          printf("Bad input 34\n");
          return -1;
        }         // not where in select
        return 0; // select *; is an OK string
      }           // not * after s
      printf("Not * after select\n");
      return -1;
    } // end selection

    //-------delete---------------
    else if (!strcmp(p.words[0], "delete")) // delete
    {
      type = DELETE;
      if (p.perstr != 1 && p.perstr != 5 && p.perstr != 9) {
        printf("Bad input 47\n");
        return -1;
      }

      if (p.perstr == 1) {
        return 0;
      } // deletes everything

      if (strcmp(p.words[1], "where")) {
        printf("Bad input 100\n");
        return -1;
      } // where must be the second
      // printf("Goes well\n");
      if (!strcmp(p.words[2], "name")) {
        if (p.perstr <= 3) {
          printf("Bad input 48\n");
          return -1;
        }

        if (!strcmp(p.words[3], "=")) {
          c_name = EQ;
        } else if (!strcmp(p.words[3], "<>")) {
          c_name = NEQ;
        } else if (!strcmp(p.words[3], "<")) {
          c_name = LT;
        } else if (!strcmp(p.words[3], ">")) {
          c_name = GT;
        } else if (!strcmp(p.words[3], "<=")) {
          c_name = LE;
        } else if (!strcmp(p.words[3], ">=")) {
          c_name = GE;
        } else if (!strcmp(p.words[3], "like")) {
          c_name = LIKE;
        }

        if (c_name == COND_NONE) {
          printf("Bad input 49\n");
          return -1;
        }

        if (p.perstr > 4) {
          init_name(p.words[4]);
          if (p.perstr <= 5)
            return 0;

          //----4------------
          if (!strcmp(p.words[5], "and")) {
            op = AND;
          } else if (!strcmp(p.words[5], "or")) {
            op = OR;
          }

          if (op == OP_NONE) {
            printf("Bad input 50\n");
            return -1;
          }

          //------5--------
          if (p.perstr <= 6) {
            printf("Bad input 51\n");
            return -1;
          }

          //-----phone--------
          if (!strcmp(p.words[6], "phone")) {
            if (p.perstr <= 7) {
              printf("Bad input 52\n");
              return -1;
            }

            if (!strcmp(p.words[7], "=")) {
              c_phone = EQ;
            } else if (!strcmp(p.words[7], "<>")) {
              c_phone = NEQ;
            } else if (!strcmp(p.words[7], "<")) {
              c_phone = LT;
            } else if (!strcmp(p.words[7], ">")) {
              c_phone = GT;
            } else if (!strcmp(p.words[7], "<=")) {
              c_phone = LE;
            } else if (!strcmp(p.words[7], ">=")) {
              c_phone = GE;
            } else if (!strcmp(p.words[7], "like")) {
              c_phone = LIKE;
            }

            if (c_phone == COND_NONE) {
              printf("Bad input 53\n");
              return -1;
            }

            if (p.perstr > 8) {
              if (!strcmp(p.words[8], "0")) {
                phone = 0;
                return 0;
              } else if (atoi(p.words[8])) {
                phone = atoi(p.words[8]);
                return 0;
              } else {
                printf("Bad input 54\n");
                return -1;
              }
            }
            // else
            printf("Bad input 55\n");
            return -1;
          }
          //----group----------
          if (!strcmp(p.words[6], "group")) {
            if (p.perstr <= 7) {
              printf("Bad input 56\n");
              return -1;
            }

            if (!strcmp(p.words[7], "=")) {
              c_group = EQ;
            } else if (!strcmp(p.words[7], "<>")) {
              c_group = NEQ;
            } else if (!strcmp(p.words[7], "<")) {
              c_group = LT;
            } else if (!strcmp(p.words[7], ">")) {
              c_group = GT;
            } else if (!strcmp(p.words[7], "<=")) {
              c_group = LE;
            } else if (!strcmp(p.words[7], ">=")) {
              c_group = GE;
            } else if (!strcmp(p.words[7], "like")) {
              c_group = LIKE;
            }

            if (c_group == COND_NONE) {
              printf("Bad input 57\n");
              return -1;
            }

            if (p.perstr > 8) {
              if (!strcmp(p.words[8], "0")) {
                group = 0;
                return 0;
              } else if (atoi(p.words[8])) {
                group = atoi(p.words[8]);
                return 0;
              } else {
                printf("Bad input 58\n");
                return -1;
              }
            }
            // else
            printf("Bad input 59\n");
            return -1;
          }
          // else
          printf("Bad input 60\n");
          return -1;

        } // p.perstr <= 3
        // else
        printf("Bad input 61\n");
        return -1;
      } // not name afte delete

      else if (!strcmp(p.words[2], "phone")) {
        if (p.perstr <= 3) {
          printf("Bad input 62\n");
          return -1;
        }

        if (!strcmp(p.words[3], "=")) {
          c_phone = EQ;
        } else if (!strcmp(p.words[3], "<>")) {
          c_phone = NEQ;
        } else if (!strcmp(p.words[3], "<")) {
          c_phone = LT;
        } else if (!strcmp(p.words[3], ">")) {
          c_phone = GT;
        } else if (!strcmp(p.words[3], "<=")) {
          c_phone = LE;
        } else if (!strcmp(p.words[3], ">=")) {
          c_phone = GE;
        } else if (!strcmp(p.words[3], "like")) {
          c_phone = LIKE;
        }

        if (c_phone == COND_NONE) {
          printf("Bad input 63\n");
          return -1;
        }

        if (p.perstr > 4) {
          // phone = p.words[3];
          if (!strcmp(p.words[4], "0")) {
            phone = 0;
          } else if (atoi(p.words[4])) {
            phone = atoi(p.words[4]);
          } else {
            printf("Bad input 64\n");
            return -1;
          }

          if (p.perstr <= 5)
            return 0;

          //----4------------
          if (!strcmp(p.words[5], "and")) {
            op = AND;
          } else if (!strcmp(p.words[5], "or")) {
            op = OR;
          }

          if (op == OP_NONE) {
            printf("Bad input 65\n");
            return -1;
          }

          //------5--------
          if (p.perstr <= 6) {
            printf("Bad input 66\n");
            return -1;
          }

          //-----name--------
          if (!strcmp(p.words[6], "name")) {
            if (p.perstr <= 7) {
              printf("Bad input 67\n");
              return -1;
            }

            if (!strcmp(p.words[7], "=")) {
              c_name = EQ;
            } else if (!strcmp(p.words[7], "<>")) {
              c_name = NEQ;
            } else if (!strcmp(p.words[7], "<")) {
              c_name = LT;
            } else if (!strcmp(p.words[7], ">")) {
              c_name = GT;
            } else if (!strcmp(p.words[7], "<=")) {
              c_name = LE;
            } else if (!strcmp(p.words[7], ">=")) {
              c_name = GE;
            } else if (!strcmp(p.words[7], "like")) {
              c_name = LIKE;
            }

            if (c_name == COND_NONE) {
              printf("Bad input 68\n");
              return -1;
            }

            if (p.perstr > 8) {
              init_name(p.words[8]);
              return 0;
            }
            // else
            printf("Bad input 75\n");
            return -1;
          }
          //----group----------
          if (!strcmp(p.words[6], "group")) {
            if (p.perstr <= 7) {
              printf("Bad input 69\n");
              return -1;
            }

            if (!strcmp(p.words[7], "=")) {
              c_group = EQ;
            } else if (!strcmp(p.words[7], "<>")) {
              c_group = NEQ;
            } else if (!strcmp(p.words[7], "<")) {
              c_group = LT;
            } else if (!strcmp(p.words[7], ">")) {
              c_group = GT;
            } else if (!strcmp(p.words[7], "<=")) {
              c_group = LE;
            } else if (!strcmp(p.words[7], ">=")) {
              c_group = GE;
            } else if (!strcmp(p.words[7], "like")) {
              c_group = LIKE;
            }

            if (c_group == COND_NONE) {
              printf("Bad input 70\n");
              return -1;
            }

            if (p.perstr > 8) {
              if (!strcmp(p.words[8], "0")) {
                group = 0;
                return 0;
              } else if (atoi(p.words[8])) {
                group = atoi(p.words[8]);
                return 0;
              } else {
                printf("Bad input 71\n");
                return -1;
              }
            }
            // else
            printf("Bad input 72\n");
            return -1;
          }
          // else
          printf("Bad input 73\n");
          return -1;

        } // p.perstr <= 3
          // else
        printf("Bad input 74\n");
        return -1;
      } // not name  nor phone after where

      else if (!strcmp(p.words[2], "group")) {
        if (p.perstr <= 3) {
          printf("Bad input 74\n");
          return -1;
        }

        if (!strcmp(p.words[3], "=")) {
          c_group = EQ;
        } else if (!strcmp(p.words[3], "<>")) {
          c_group = NEQ;
        } else if (!strcmp(p.words[3], "<")) {
          c_group = LT;
        } else if (!strcmp(p.words[3], ">")) {
          c_group = GT;
        } else if (!strcmp(p.words[3], "<=")) {
          c_group = LE;
        } else if (!strcmp(p.words[3], ">=")) {
          c_group = GE;
        } else if (!strcmp(p.words[3], "like")) {
          c_group = LIKE;
        }

        if (c_group == COND_NONE) {
          printf("Bad input 75\n");
          return -1;
        }

        if (p.perstr > 4) {
          // group = p.words[3];
          if (!strcmp(p.words[4], "0")) {
            group = 0;
          } else if (atoi(p.words[4])) {
            group = atoi(p.words[4]);
          } else {
            printf("Bad input 76\n");
            return -1;
          }

          if (p.perstr <= 5)
            return 0;

          //----4------------
          if (!strcmp(p.words[5], "and")) {
            op = AND;
          } else if (!strcmp(p.words[5], "or")) {
            op = OR;
          }

          if (op == OP_NONE) {
            printf("Bad input 77\n");
            return -1;
          }

          //------5--------
          if (p.perstr <= 6) {
            printf("Bad input 78\n");
            return -1;
          }

          //-----name--------
          if (!strcmp(p.words[6], "name")) {
            if (p.perstr <= 7) {
              printf("Bad input 79\n");
              return -1;
            }

            if (!strcmp(p.words[7], "=")) {
              c_name = EQ;
            } else if (!strcmp(p.words[7], "<>")) {
              c_name = NEQ;
            } else if (!strcmp(p.words[7], "<")) {
              c_name = LT;
            } else if (!strcmp(p.words[7], ">")) {
              c_name = GT;
            } else if (!strcmp(p.words[7], "<=")) {
              c_name = LE;
            } else if (!strcmp(p.words[7], ">=")) {
              c_name = GE;
            } else if (!strcmp(p.words[7], "like")) {
              c_name = LIKE;
            }

            if (c_name == COND_NONE) {
              printf("Bad input 80\n");
              return -1;
            }

            if (p.perstr > 8) {
              init_name(p.words[8]);
              return 0;
            }
            // else
            printf("Bad input 81\n");
            return -1;
          }
          //----phone----------
          if (!strcmp(p.words[6], "phone")) {
            if (p.perstr <= 7) {
              printf("Bad input 82\n");
              return -1;
            }

            if (!strcmp(p.words[7], "=")) {
              c_phone = EQ;
            } else if (!strcmp(p.words[7], "<>")) {
              c_phone = NEQ;
            } else if (!strcmp(p.words[7], "<")) {
              c_phone = LT;
            } else if (!strcmp(p.words[7], ">")) {
              c_phone = GT;
            } else if (!strcmp(p.words[7], "<=")) {
              c_phone = LE;
            } else if (!strcmp(p.words[7], ">=")) {
              c_phone = GE;
            } else if (!strcmp(p.words[7], "like")) {
              c_phone = LIKE;
            }

            if (c_phone == COND_NONE) {
              printf("Bad input 83\n");
              return -1;
            }

            if (p.perstr > 8) {
              if (!strcmp(p.words[8], "0")) {
                phone = 0;
                return 0;
              } else if (atoi(p.words[8])) {
                phone = atoi(p.words[8]);
                return 0;
              } else {
                printf("Bad input 84\n");
                return -1;
              }
            }
            // else
            printf("Bad input 85\n");
            return -1;
          }
          // else
          printf("Bad input 86\n");
          return -1;

        } // p.perstr <= 5
        // else
        printf("Bad input 87\n");
        return -1;
      } // not name  nor phone nor group after where
      printf("Bad input 88\n");
      return -1;
    } // end delete

    // insert
    else if (!strcmp(p.words[0], "insert")) {
      type = INSERT;
      if (p.perstr != 4) {
        printf("Bad input 89\n");
        return -1;
      }

      init_name(p.words[1]);

      if (!strcmp(p.words[2], "0")) {
        phone = 0;
      } else if (atoi(p.words[2])) {
        phone = atoi(p.words[2]);
      } else {
        printf("Bad input 90\n");
        return -1;
      }

      if (!strcmp(p.words[3], "0")) {
        group = 0;
        return 0;
      } else if (atoi(p.words[3])) {
        group = atoi(p.words[3]);
        return 0;
      } else {
        printf("Bad input 91\n");
        return -1;
      }
    }

    // quit
    else if (!strcmp(p.words[0], "quit")) {
      type = QUIT;
      if (p.perstr != 1) {
        printf("Bad input 92\n");
        return -1;
      }

      return 0;
    }
  } // perstr = 0
  return 0;
} // end parser

void command::nullify() {
  type = CMD_NONE;
  c_name = COND_NONE;
  c_phone = COND_NONE;
  c_group = COND_NONE;
  op = OP_NONE;
  phone = group = -1;
}

//------prints-------
int command::print(FILE *out) {
  switch (type) {
  case CMD_NONE:
    fprintf(out, "\n");
    return 1;
  case QUIT:
    fprintf(out, "quit\n");
    return 0;
  case STOP:
    fprintf(out, "_stop_\n");
    return 0;
  case SELECT: //---------------- SELECT -----------------
    fprintf(out, "select *");

    if (c_name == COND_NONE && c_phone == COND_NONE && c_group == COND_NONE) {
      printf("bad\n");
      fprintf(out, "\n");
      return 0;
    }
    fprintf(out, " where ");

    switch (c_name) {
    case COND_NONE:
      break;
    case EQ:
      fprintf(out, "name = %s", get_name());
      break;
    case NEQ:
      fprintf(out, "name <> %s", get_name());
      break;
    case LT:
      fprintf(out, "name < %s", get_name());
      break;
    case GT:
      fprintf(out, "name > %s", get_name());
      break;
    case LE:
      fprintf(out, "name <= %s", get_name());
      break;
    case GE:
      fprintf(out, "name >= %s", get_name());
      break;
    case LIKE:
      fprintf(out, "name like %s", get_name());
      break;
    }

    switch (op) {
    case OP_NONE:
      break;
    case AND:
      if (c_name != COND_NONE)
        fprintf(out, " and ");
      break;
    case OR:
      if (c_name != COND_NONE)
        fprintf(out, " or ");
      break;
    }

    switch (c_phone) {
    case COND_NONE:
      break;
    case EQ:
      fprintf(out, "phone = %d", phone);
      break;
    case NEQ:
      fprintf(out, "phone <> %d", phone);
      break;
    case LT:
      fprintf(out, "phone < %d", phone);
      break;
    case GT:
      fprintf(out, "phone > %d", phone);
      break;
    case LE:
      fprintf(out, "phone <= %d", phone);
      break;
    case GE:
      fprintf(out, "phone >= %d", phone);
      break;
    case LIKE:
      fprintf(out, "R U CRAZY???\nLIKE WITH INTEGERS IS SaCrElIg0u$\n");
      return 1;
    }

    switch (op) {
    case OP_NONE:
      break;
    case AND:
      if (c_phone != COND_NONE && c_name == COND_NONE)
        fprintf(out, " and ");
      break;
    case OR:
      if (c_phone != COND_NONE && c_name == COND_NONE)
        fprintf(out, " or ");
      break;
    }

    switch (c_group) {
    case COND_NONE:
      break;
    case EQ:
      fprintf(out, "group = %d", group);
      break;
    case NEQ:
      fprintf(out, "group <> %d", group);
      break;
    case LT:
      fprintf(out, "group < %d", group);
      break;
    case GT:
      fprintf(out, "group > %d", group);
      break;
    case LE:
      fprintf(out, "group <= %d", group);
      break;
    case GE:
      fprintf(out, "group >= %d", group);
      break;
    case LIKE:
      fprintf(out, "R U CRAZY???\nLIKE WITH INTEGERS IS SaCrElIg0u$\n");
      return 1;
    }
    break;
  case INSERT:
    fprintf(out, "insert (%s, %d, %d)", get_name(), phone, group);
    break;
  case DELETE: //-------------------DELETE-------------------------
    fprintf(out, "delete");

    if (c_name == COND_NONE && c_phone == COND_NONE && c_group == COND_NONE) {
      fprintf(out, "\n");
      return 0;
    }
    fprintf(out, " where ");

    switch (c_name) {
    case COND_NONE:
      break;
    case EQ:
      fprintf(out, "name = %s", get_name());
      break;
    case NEQ:
      fprintf(out, "name <> %s", get_name());
      break;
    case LT:
      fprintf(out, "name < %s", get_name());
      break;
    case GT:
      fprintf(out, "name > %s", get_name());
      break;
    case LE:
      fprintf(out, "name <= %s", get_name());
      break;
    case GE:
      fprintf(out, "name >= %s", get_name());
      break;
    case LIKE:
      fprintf(out, "name like %s", get_name());
      break;
    }

    switch (op) {
    case OP_NONE:
      break;
    case AND:
      if (c_name != COND_NONE)
        fprintf(out, " and ");
      break;
    case OR:
      if (c_name != COND_NONE)
        fprintf(out, " or ");
      break;
    }

    switch (c_phone) {
    case COND_NONE:
      break;
    case EQ:
      fprintf(out, "phone = %d", phone);
      break;
    case NEQ:
      fprintf(out, "phone <> %d", phone);
      break;
    case LT:
      fprintf(out, "phone < %d", phone);
      break;
    case GT:
      fprintf(out, "phone > %d", phone);
      break;
    case LE:
      fprintf(out, "phone <= %d", phone);
      break;
    case GE:
      fprintf(out, "phone >= %d", phone);
      break;
    case LIKE:
      fprintf(out, "R U CRAZY???\nLIKE WITH INTEGERS IS SaCrElIg0u$\n");
      return 1;
    }

    switch (op) {
    case OP_NONE:
      break;
    case AND:
      if (c_phone != COND_NONE && c_name == COND_NONE)
        fprintf(out, " and ");
      break;
    case OR:
      if (c_phone != COND_NONE && c_name == COND_NONE)
        fprintf(out, " or ");
      break;
    }

    switch (c_group) {
    case COND_NONE:
      break;
    case EQ:
      fprintf(out, "group = %d", group);
      break;
    case NEQ:
      fprintf(out, "group <> %d", group);
      break;
    case LT:
      fprintf(out, "group < %d", group);
      break;
    case GT:
      fprintf(out, "group > %d", group);
      break;
    case LE:
      fprintf(out, "group <= %d", group);
      break;
    case GE:
      fprintf(out, "group >= %d", group);
      break;
    case LIKE:
      fprintf(out, "R U CRAZY???\nLIKE WITH INTEGERS IS SaCrElIg0u$\n");
      return 1;
    }
    break;
  }
  fprintf(out, ";\n");
  return 0;
}

void command::print_hat(FILE *out) {
  fprintf(out,
          "     name    phone   group   cmd    c_name  c_ph   c_gr   oper\n");
}

void command::print_cmd(FILE *out) {
  // printf("sdfgsdg\n");
  if (!get_name())
    fprintf(out, "      NULL");
  else
    fprintf(out, "%10s", get_name());

  fprintf(out, "%9d", phone);
  fprintf(out, "  %4d   ", group);

  switch (type) {
  case CMD_NONE:
    fprintf(out, "empty   ");
    break;
  case QUIT:
    fprintf(out, "quit    ");
    break;
  case STOP:
    fprintf(out, "stop    ");
    break;
  case SELECT:
    fprintf(out, "select  ");
    break;
  case INSERT:
    fprintf(out, "insert  ");
    break;
  case DELETE:
    fprintf(out, "delete  ");
    break;
  }

  switch (c_name) {
  case COND_NONE:
    fprintf(out, " none ");
    break;
  case EQ:
    fprintf(out, "  =   ");
    break;
  case NEQ:
    fprintf(out, "  <>  ");
    break;
  case LT:
    fprintf(out, "  <   ");
    break;
  case GT:
    fprintf(out, "  >   ");
    break;
  case LE:
    fprintf(out, "  <=  ");
    break;
  case GE:
    fprintf(out, "  >=  ");
    break;
  case LIKE:
    fprintf(out, " like ");
    break;
  }

  switch (c_phone) {
  case COND_NONE:
    fprintf(out, "  none ");
    break;
  case EQ:
    fprintf(out, "   =   ");
    break;
  case NEQ:
    fprintf(out, "   <>  ");
    break;
  case LT:
    fprintf(out, "   <   ");
    break;
  case GT:
    fprintf(out, "   >   ");
    break;
  case LE:
    fprintf(out, "   <=  ");
    break;
  case GE:
    fprintf(out, "   >=  ");
    break;
  case LIKE:
    fprintf(out, "  like ");
    break;
  }

  switch (c_group) {
  case COND_NONE:
    fprintf(out, "  none ");
    break;
  case EQ:
    fprintf(out, "   =   ");
    break;
  case NEQ:
    fprintf(out, "   <>  ");
    break;
  case LT:
    fprintf(out, "   <   ");
    break;
  case GT:
    fprintf(out, "   >   ");
    break;
  case LE:
    fprintf(out, "   <=  ");
    break;
  case GE:
    fprintf(out, "   >=  ");
    break;
  case LIKE:
    fprintf(out, "  like ");
    break;
  }

  switch (op) {
  case OP_NONE:
    fprintf(out, "  none");
    break;
  case AND:
    fprintf(out, "   and");
    break;
  case OR:
    fprintf(out, "   or");
    break;
  }

  fprintf(out, "\n");
}

//--------------------PARSE-------------------

int parse::init_words() {
  words = new char *[(LEN + 1) / 2];
  if (!words)
    return -1;
  return 0;
}

int parse::init(char *nbuf, char *buf, int *sp) {
  if (!buf || !*buf)
    return 0;

  int j = 0, i = 0;

  for (;;) {
    for (; buf[i] && sp[(unsigned int)buf[i]]; i++) // space
    {
      nbuf[i] = 0;
    }
    if (!buf[i]) {
      nbuf[i] = 0;
      break;
    }
    words[j] = nbuf + i;
    j++;

    for (; buf[i] && !sp[(unsigned int)buf[i]]; i++) // non-space
    {
      nbuf[i] = buf[i];
    }
    if (!buf[i]) {
      nbuf[i] = 0;
      break;
    }
  }

  perstr = j;
  return 0;
}

void parse::destroy() {
  perstr = 0;

  if (words) {
    delete[] words;
    words = nullptr;
  }
}

void parse::print() {
  if (perstr == 0) {
    printf("No words somehow\n");
    return;
  }

  int max_pr = MAX_PRINT > perstr ? perstr : MAX_PRINT;
  for (int i = 0; i < max_pr; i++)
    printf("words[%d] = \"%s\"\n", i, words[i]);
}
