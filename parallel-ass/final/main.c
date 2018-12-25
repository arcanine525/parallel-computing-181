#include <stdlib.h>
#include <stdio.h>
#include "transaction.h"
#include "apriori.h"

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: ./apriori <threshold> <num transactions>\n");
    return EXIT_FAILURE;
  }

  int support_threshold = atoi(argv[1]);
  int num_transactions = atoi(argv[2]);
  printf("using support threshold %d\n", support_threshold);

  struct trans_list t_list = generate_trans_list(num_transactions);
  print_trans_list(t_list);

  struct occurr_table *o_table = malloc(sizeof(struct occurr_table));

  // first run through will have 9 items (1,2,3,...,9)
  o_table->occurr = malloc(sizeof(struct occurrence) * 10);
  o_table->threshold = support_threshold;
  o_table->length = 10;

  for (int i = 0; i < 10; ++i) {
    o_table->occurr[i].item = malloc(sizeof(struct item));

    // item sets will be size one at the start
    o_table->occurr[i].item->i = malloc(sizeof(int) * 1);

    o_table->occurr[i].item->i[0] = i;
    o_table->occurr[i].item->length = 1;
  }

  fill_occurrence_table(o_table, t_list);
  print_occurr_table(*o_table);

  while(1) {
    struct occurr_table *new_occurr_table = malloc(sizeof(struct occurr_table));
    printf("next round\n");
    int res = next_round(o_table, new_occurr_table);
    printf("fill table\n");
    fill_occurrence_table(new_occurr_table, t_list);
    printf("print table\n");
    print_occurr_table(*new_occurr_table);

    if (res == 0) {
      break;
    }

    o_table = new_occurr_table;
  }

  return EXIT_SUCCESS;
}
