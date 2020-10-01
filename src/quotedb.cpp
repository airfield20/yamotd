//
// Created by aaron on 2020-10-01.
//

#include "quotedb.h"

cis427::quotedb::quotedb(const string& path_to_quotes) {
    std::srand(std::time(nullptr));
    m_file.open(path_to_quotes);
    string line = "";
    string quote = "";
    while(std::getline(m_file, line)){
        if(line.at(0) == '-' && line.at(1) == '-' && quote != ""){
            quote.append(line);
            m_quotes.push_back(quote);
            quote = "";
        }
        else{
            quote.append(line);
        }
    }
}

string cis427::quotedb::get_random_quote() {
    return get_quote(std::rand() % m_quotes.size());
}

string cis427::quotedb::get_quote(const unsigned long &index) {
    return std::__cxx11::string();
}
