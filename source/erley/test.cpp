#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <erley/erley.h>
#include <iostream>
#include <utility>
#include <vector>
#include <set>


TEST_CASE("correct_bracket_sequence") {
  std::set<std::string> grammar;
  grammar.insert("S->(S)");
  grammar.insert("S->SS");
  grammar.insert("S->");

  Erley erley_algo;
  erley_algo.set_grammar(grammar);
  
  CHECK(erley_algo.predict("") == true);
  CHECK(erley_algo.predict("()") == true);
  CHECK(erley_algo.predict("()()") == true);

  CHECK(erley_algo.predict("(()())()") == true);
  CHECK(erley_algo.predict("(()())") == true);
  CHECK(erley_algo.predict("()()()()()()()()()") == true);
  CHECK(erley_algo.predict("((((()))))()") == true);
  CHECK(erley_algo.predict("()(())(()())(()()(()(()()((()())())())))") == true);

  CHECK(erley_algo.predict("(") == false);
  CHECK(erley_algo.predict(")") == false);
  CHECK(erley_algo.predict(")(") == false);

  CHECK(erley_algo.predict("(((((((((((") == false);
  // CHECK(erley_algo.predict("))))))))))))") == false);
  CHECK(erley_algo.predict(")()()()()()(()())(") == false);
  CHECK(erley_algo.predict("()()()()()(()())(") == false);
  CHECK(erley_algo.predict("()()()()(()(()())") == false);
}


TEST_CASE("arithmetic") {
  std::set<std::string> grammar;

  grammar.insert("S->SOS");
  grammar.insert("S->(S)");
  grammar.insert("S->0");
  grammar.insert("S->DN");

  grammar.insert("O->+");
  grammar.insert("O->-");
  grammar.insert("O->*");
  grammar.insert("O->/");

  grammar.insert("D->1");
  grammar.insert("D->2");
  grammar.insert("D->3");
  grammar.insert("D->4");
  grammar.insert("D->5");
  grammar.insert("D->6");
  grammar.insert("D->7");
  grammar.insert("D->8");
  grammar.insert("D->9");

  grammar.insert("N->0");
  grammar.insert("N->1");
  grammar.insert("N->2");
  grammar.insert("N->3");
  grammar.insert("N->4");
  grammar.insert("N->5");
  grammar.insert("N->6");
  grammar.insert("N->7");
  grammar.insert("N->8");
  grammar.insert("N->9");

  grammar.insert("N->NN");
  grammar.insert("N->");

  Erley erley_algo;
  erley_algo.set_grammar(grammar);
  
  CHECK(erley_algo.predict("0") == true);
  CHECK(erley_algo.predict("1") == true);
  CHECK(erley_algo.predict("3546789826") == true);

  CHECK(erley_algo.predict("1+1") == true);
  CHECK(erley_algo.predict("2-2") == true);
  CHECK(erley_algo.predict("3*3") == true);
  CHECK(erley_algo.predict("4/4") == true);

  CHECK(erley_algo.predict("(1)") == true);
  CHECK(erley_algo.predict("(1+9)") == true);
  CHECK(erley_algo.predict("(1+8)-54678") == true);
  CHECK(erley_algo.predict("(456-2387)*65392/8298") == true);

  CHECK(erley_algo.predict("1+2+3+3+879+3") == true);

  CHECK(erley_algo.predict("((1+9)-1)*(5-4-2)/(2/(90-0)+(0-32))") == true);
}


TEST_CASE("smth_random") {
  std::set<std::string> grammar;

  grammar.insert("S->T+S");
  grammar.insert("S->T");
  grammar.insert("T->F*T");
  grammar.insert("T->F");
  grammar.insert("F->(S)");
  grammar.insert("F->a");

  Erley erley_algo;
  erley_algo.set_grammar(grammar);
  
  CHECK(erley_algo.predict("(a+a)") == true);

}
