#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define CNRM "\x1b[0m"
#define CRED "\x1b[31m"
#define CGRN "\x1b[32m"

int stringsum(char *s);
int distance_between(char *s, char c);
char *string_between(char *s, char c);
void stringsum2(char *s, int *res);

static int test_num = 1;

static void logger(int passed, char *s)
{
   char *res;
   char *color;

   if (passed) {
      res = "PASS";
      color = CGRN;
   } else {
      res = "FAIL";
      color = CRED;
   }
   printf("[Test %d][%s%s%s] %s\n", test_num++, color, res, CNRM, s);
}

static void test_stringsum(char *input, int expected)
{
   int test;
   char buf[256] = { 0 };

   test = stringsum(input);
   sprintf(buf, "Returned: %d, Expected: %d", test, expected);
   logger(test == expected, buf);
}

static void test_distance_between(char *str, char c, int expected)
{
   int test;
   char buf[256] = { 0 };

   test = distance_between(str, c);
   sprintf(buf, "Returned: %d, Expected: %d", test, expected);
   logger(test == expected, buf);
}

static void test_string_between(char *str, char c, const char *expected)
{
   char *res_char;
   char buf[256] = { 0 };

   res_char = string_between(str, c);
   snprintf(buf, sizeof(buf), "Returned: %s, Expected: %s", res_char, expected);

   if (!res_char && expected) {
      logger(0, buf);
   } else {
      if (!expected)
         logger(!res_char, buf);
      else
         logger(!strcmp(res_char, expected), buf);
      free(res_char);
   }
}

static void test_stringsum2(char *input, int expected)
{
   int res_int;
   char buf[256] = { 0 };

   stringsum2(input, &res_int);
   sprintf(buf, "Returned: %d, Expected: %d", res_int, expected);
   logger(res_int == expected, buf);
}

int main(void)
{
   printf("Testing stringsum()\n");
   test_stringsum("Lorem ipsum dolor sit amet", 292);
   test_stringsum("L0rem 1psum d0l0r s1t amet", -1);
   test_stringsum("", 0);

   test_num = 1;
   printf("\nTesting distance_between()\n");
   test_distance_between("Lorem ipsum dolor sit amet", 'm', 19);
   test_distance_between("Lorem ipsum dolor sit amet", 'u', 0);
   test_distance_between("Lorem ipsum dolor sit amet", 'y', -1);
   test_distance_between("", 'z', -1);

   test_num = 1;
   printf("\nTesting string_between()\n");
   test_string_between("Lorem ipsum dolor sit amet", 'o', "rem ipsum dol");
   test_string_between("Lorem ipsum dolor sit amet", 'a', "");
   test_string_between("Lorem ipsum dolor sit amet", 'y', NULL);
   test_string_between("", 'z', NULL);

   test_num = 1;
   printf("\nTesting stringsum2()\n");
   test_stringsum2("Lorem ipsum dolor sit amet", 292);
   test_stringsum2("L0rem 1psum d0l0r s1t amet", -1);
   test_stringsum2("", 0);
   
   return 0;
}