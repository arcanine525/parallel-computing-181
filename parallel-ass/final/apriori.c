#include <stdlib.h>
#include <stdio.h>

#include "apriori.h"
#include "transaction.h"


void fill_occurrence_table(struct occurr_table *o_table, struct trans_list t_list) {
#pragma omp parallel
  for (int i = 0; i < t_list.t_size; ++i) {
    struct trans_entry t_entry = t_list.t_entry[i];
    for (int j = 0; j < o_table->length; ++j) {
      struct occurrence occurr = o_table->occurr[j];
      struct item *item = occurr.item;

      // assume everything will be found, it if isn't then we don't increase
      // support
      int found_all = 1;

      int *found = malloc(sizeof(int) * item->length);
      for (int k = 0; k < item->length; ++k) {
        int check = item->i[k];
        found[k] = 0; // assume we don't find what we are looking for

        for (int l = 0; l < t_entry.d_size; ++l) {
          int trans_id = t_entry.dataset[l];
          if (trans_id == check) {
            found[k] = 1;
          }
        }
      }

      for (int ix = 0; ix < item->length; ++ix) {
        if (found[ix] == 0) {
          // we didnt find something, give up
          found_all = 0;
        }
      }

      if (found_all == 1) {
        o_table->occurr[j].support++;
      }
    }
  }
}

int next_round(struct occurr_table *o_table, struct occurr_table *new_occurr_table) {
  new_occurr_table->threshold = o_table->threshold;
  new_occurr_table->occurr = malloc(sizeof(struct occurrence));
  new_occurr_table->length = 0;

  int threshold = o_table->threshold;
  int new_length = 0;

#pragma omp parallel
  for (int i = 0; i < o_table->length; ++i) {
    int i_support = o_table->occurr[i].support;
    if (i_support < threshold) {
      /*
       * if the i support is not met at this point, then combining the
       * number with something else will also not be supported and is thus
       * not worth pursing
       */
      continue;
    }

    for (int j = i + 1; j < o_table->length; ++j) {
      int j_support = o_table->occurr[j].support;

      if (j_support < threshold) {
        // not worth pursuing .. explained above
        continue;
      }

      struct item *i_piece = o_table->occurr[i].item;
      struct item *j_piece = o_table->occurr[j].item;

      // if j adds nothing new to i then we will skip
      int *added = malloc(sizeof(int) * j_piece->length);
      for (int ij = 0; ij < j_piece->length; ++ij) {
        int piece = j_piece->i[ij];
        int found = 0;
        added[ij] = 0; // assume not found
        for (int ii = 0; ii < i_piece->length; ++ii) {
          if (piece == i_piece->i[ii]) {
            // we have found the i piece within j
            found = 1;
          }
        }

        if (found == 1) {
          added[ij] = 1;
        }
      }

      int anything_unique = 0;
      for (int ii = 0; ii < j_piece->length; ++ii) {
        if (added[ii] == 0) {
          anything_unique = 1;
        }
      }

      if (anything_unique == 0) {
        continue;
      }

      int item_size = i_piece->length + j_piece->length;
      struct item *item = malloc(sizeof(struct item));
      item->i = malloc(sizeof(int) * item_size);
      item->length = item_size;

      for (int ix = 0; ix < i_piece->length; ++ix) {
        item->i[ix] = i_piece->i[ix];
      }

      for (int ix = i_piece->length; ix < item_size; ++ix) {
        item->i[ix] = j_piece->i[ix - i_piece->length];
      }

      new_occurr_table->length++;
      new_occurr_table->occurr = realloc(new_occurr_table->occurr, (new_occurr_table->length) * sizeof(struct occurrence));

      struct occurrence *occurr = malloc(sizeof(occurrence));
      occurr->item = malloc(sizeof(struct item));
      occurr->item = item;
      new_occurr_table->occurr[new_occurr_table->length - 1] = *occurr;
    }
  }

  return new_occurr_table -> length;
}

void print_occurr_table(struct occurr_table o_table) {
  
  printf("|ITEM|\t|SUPPORT|\n");

  for (int i = 0; i < o_table.length; ++i) {
    struct item *item = o_table.occurr[i].item;
    int support = o_table.occurr[i].support;
    if (support < o_table.threshold) {
      continue;
    }

    printf("{");


    for (int j = 0; j < item->length; ++j) {
      printf("%d,", item->i[j]);
  
    }

    printf("}");
    

    printf("\t%d\n", support);

  }
  
}
