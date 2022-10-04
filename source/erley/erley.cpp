#include <erley/erley.h>

#include <iostream>
#include <utility>
#include <vector>
#include <set>


Rule::Rule(char nonterminal_from, std::string to) :
        nonterminal_from(nonterminal_from), to(std::move(to)) {}


Rule::Rule(std::string rule_string) 
{
    std::string new_string;
    for (int i = 3; i < rule_string.size(); ++i)
    {
        new_string += rule_string[i];
    }
    nonterminal_from = rule_string[0];
    to = new_string;
}


bool operator<(const Rule& first, const Rule& second) 
{
    return first.nonterminal_from < second.nonterminal_from 
            || (first.nonterminal_from == second.nonterminal_from && first.to < second.to);
}


bool operator==(const Rule& first, const Rule& second) 
{
    return !(first < second) && !(second < first);
}


Situation::Situation(Rule rule, int point_index, int begin_symb_numb) :
        rule(std::move(rule)), point_index(point_index), begin_symb_numb(begin_symb_numb) {}



bool operator<(const Situation& first, const Situation& second) 
{
    return first.rule < second.rule || (first.rule == second.rule && first.point_index < second.point_index ||
                                        (first.point_index == second.point_index && first.begin_symb_numb < second.begin_symb_numb));
}


bool operator==(const Situation& first, const Situation& second) 
{
    return !(first < second) && !(second < first);
}


void Erley::set_grammar(const std::set<std::string>& new_grammar) 
{
    grammar.clear();
    for (auto rule_string : new_grammar) 
    {
        grammar.insert(Rule(rule_string));
    }
}


bool Erley::predict(const std::string& word) 
{
    D.resize(word.size() + 1);
    D[0].insert(Situation(Rule('\0', "S"), 0, 0));
    do {
        int size_before = D[0].size();
        Predict(0);
        Complete(0);
        if (size_before == D[0].size()) 
        {
            break;
        }
    } while (true);
    for (int j = 1; j <= word.size(); ++j) 
    {
        Scan(j - 1, word[j - 1]);
        do {
            int size_before = D[j].size();
            Predict(j);
            Complete(j);
            if (size_before == D[j].size()) {
                break;
            }
        } while (true);
    }
    return D[word.size()].find(Situation(Rule('\0', "S"), 1, 0)) != D[word.size()].end();
}


bool Erley::Predict(int D_index) {
    std::set<Situation> new_situations;
    for (Situation situation : D[D_index]) 
    {
        for (Rule rule : grammar) 
        {
            if (rule.nonterminal_from == situation.rule.to[situation.point_index]) 
            {
                Situation new_situation = Situation(rule, 0, D_index);
                if (D[D_index].find(new_situation) == D[D_index].end()) 
                {
                    new_situations.insert(new_situation);
                }
            }
        }
    }
    for (Situation situation : new_situations) 
    {
        D[D_index].insert(situation);
    }
    return !new_situations.empty();
}


bool Erley::Scan(int D_index, char symbol) 
{
    std::set<Situation> new_situations;
    for (Situation situation : D[D_index]) 
    {
        if (symbol == situation.rule.to[situation.point_index]) 
        {
            Situation new_situation = Situation(situation.rule, situation.point_index + 1, situation.begin_symb_numb);
            if (D[D_index + 1].find(new_situation) == D[D_index + 1].end()) 
            {
                new_situations.insert(new_situation);
            }
        }
    }
    for (Situation situation : new_situations) 
    {
        D[D_index + 1].insert(situation);
    }
    return !new_situations.empty();
}


bool Erley::Complete(int D_index) 
{
    std::set<Situation> new_situations;
    for (const Situation& under_situation : D[D_index]) 
    {
        if (under_situation.point_index != under_situation.rule.to.size()) 
        {
            continue;
        }
        for (const Situation& upper_situation : D[under_situation.begin_symb_numb]) 
        {
            if (upper_situation.rule.to[upper_situation.point_index] == under_situation.rule.nonterminal_from) 
            {
                Situation new_situation = Situation(upper_situation.rule, upper_situation.point_index + 1, upper_situation.begin_symb_numb);
                if (D[D_index].find(new_situation) == D[D_index].end()) 
                {
                    new_situations.insert(new_situation);
                }
            }
        }
    }
    for (Situation situation : new_situations) 
    {
        D[D_index].insert(situation);
    }
    return !new_situations.empty();
}

