// Macroses for Unit Testing

#include <iostream>

#define CHECK_EQUAL(actual, expected)                            \
  if(! (actual == expected) )                                    \
  {                                                              \
    std::cout << "CHECK_EQUAL failed! "                          \
              << "File: " << __FILE__ << " "                     \
              << "Line: " << __LINE__ << " "                     \
              << "!( " << #actual << " == " << #expected << ") " \
              << #actual   << " == " << actual << " "            \
              << #expected << " == " << expected << std::endl;   \
    ret = 1;                                                     \
  }

#define CHECK_TRUE(expr)                        \
  if (!(expr))                                  \
  {                                             \
    std::cout << "CHECK_TRUE failed! "          \
              << "File: " << __FILE__ << " "    \
              << "Line: " << __LINE__ << " "    \
              << "expr=" << #expr << std::endl; \
    ret = 1;                                    \
  }

#define CHECK_FALSE(expr)                       \
  if ((expr))                                   \
  {                                             \
    std::cout << "CHECK_FALSE failed! "         \
              << "File: " << __FILE__ << " "    \
              << "Line: " << __LINE__ << " "    \
              << "expr=" << #expr << std::endl; \
    ret = 1;                                    \
  }

