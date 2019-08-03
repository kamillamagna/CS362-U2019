int asserttrue(int actual, int expected, char* title, int pre) {
  printf("\tTesting %s ... ", title);
  if (expected == actual) printf("PASS\n");
  else printf("FAIL\n");
  printf("\t\tpre: %i\n", pre);
  printf("\t\texpected: %i\n", expected);
  printf("\t\tactual: %i\n\n", actual);
  return 0;
}
