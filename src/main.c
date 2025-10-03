#include <stdio.h>
#include <glib.h>

int main(){

  GHashTable *ht = g_hash_table_new(g_str_hash, g_str_equal);
  g_hash_table_insert(ht, "key1", "value1");

  int a = 20;
  int b = 22;
  char* h = "GHsadfsedf";
  printf("Hello!");
return 0;
}
