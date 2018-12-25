#ifndef __TRANSACTION_H_
#define __TRANSACTION_H_

// transaction entry (ie a single transaction)
typedef struct trans_entry {
  int *dataset;
  int d_size;
} trans_entry;

// list of all transactions
typedef struct trans_list {
  struct trans_entry *t_entry;
  int t_size;
  int t_index;
} transaction_list;


struct trans_entry new_trans_entry(int *trans, int size);
void print_trans_entry(struct trans_entry t_entry);

void add_trans(struct trans_list *t_list, struct trans_entry t_entry);
void print_trans_list(struct trans_list t_list);
struct trans_list generate_trans_list(int num_transactions);


#endif
