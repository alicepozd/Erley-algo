/*!
 * @mainpage Алгоритм Эрли
 * 
 * @section intro_sec Введение
 *
 * Алгоритм Эрли можно применять для парсинга формальных языков, задаваемых достаточно простой грамматикой. 
 * Например это регулярные выражения, математические выражения, для имеющих простую структуры форматов данных
 * 
 * @section install_sec Как пользоваться
 * 
 * @code
 * std::set<std::string> grammar;

 * grammar.insert("S->T+S");
 * grammar.insert("S->T");
 * grammar.insert("T->F*T");
 * grammar.insert("T->F");
 * grammar.insert("F->(S)");
 * grammar.insert("F->a");

 * Erley erley_algo;
 * erley_algo.set_grammar(grammar);
  
 * erley_algo.predict("(a+a)");
 * @endcode
 * 
 * @section classes_sec Основной класс
 * 
 * @ref Erley
 * 
 * <a href="https://github.com/alicepozd/Erley-algo/blob/main/Erley_algo.pdf"> ссылка на отчет </a>
 */



#ifndef ERLEY_H
#define ERLEY_H


#include <iostream>
#include <utility>
#include <vector>
#include <set>


struct Rule 
{
    Rule(char nonterminal_from, std::string to);
    Rule(std::string to);

    char nonterminal_from;
    std::string to;
};


bool operator<(const Rule& first, const Rule& second);
bool operator==(const Rule& first, const Rule& second);


struct Situation 
{
    Situation(Rule  rule, int point_index, int begin_symb_numb);

    Rule rule;
    int point_index;
    int begin_symb_numb;
};


bool operator<(const Situation& first, const Situation& second);
bool operator==(const Situation& first, const Situation& second);


/*! @brief Класс реализует алгоритм Эрли
 *
 * Класс реализует алгоритм, осуществляющий парсинг слова в условиях заданной КС грамматики и определяющий, принадлежит ли слово ей
 */
class Erley 
{
public:
    Erley() = default;

    /*!
     * @param new_grammar грамматика 
     * @note Грамаматика задается в виде множества правил, записанных ввиде строк вида A->alpha, A - нетерминал, alpha - последовательность терминальных и нетерминальных символов
     */
    void set_grammar(const std::set<std::string>& new_grammar);

    /*!
     * @param word слово из терминальных символов
     * @return 0, если слово не лежит в грамматике, и 1 иначе
     */
    bool predict(std::string word);
private:
    bool Predict(int D_index);

    bool Scan(int D_index, char symbol);

    bool Complete(int D_index);

    std::vector<std::set<Situation>> D;
    std::set<Rule> grammar;
};


#endif
