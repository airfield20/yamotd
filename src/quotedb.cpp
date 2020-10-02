//
// Created by aaron on 2020-10-01.
//

#include "../include/server/quotedb.h"

cis427::QuoteDB::QuoteDB(const std::string& path_to_quotes) {
    std::srand(std::time(NULL));
    m_file.open(path_to_quotes);
    if(!m_file.is_open()){
        throw 1;
    }
    std::string line;
    std::string quote;
    while(std::getline(m_file, line)){
        if(!line.empty() && line.at(0) == '-' && line.at(1) == '-' && !quote.empty()){
            quote.push_back('\n');
            quote.append(line);
            m_quotes.push_back(quote);
            quote = "";
        }
        else{
            if(!line.empty()) {
                if (!quote.empty()) {
                    quote.push_back('\n');
                }
                quote.append(line);
            }
        }
    }
}

cis427::QuoteDB::QuoteDB() : m_quotes({}) {}

std::string cis427::QuoteDB::get_random_quote() {
    return m_quotes.at(std::rand() % m_quotes.size());
}

std::string cis427::QuoteDB::get_quote(const unsigned long &index) {
    if(index < m_quotes.size()) {
        return m_quotes.at(index);
    }
    throw 2;
}

int cis427::QuoteDB::get_num_quotes() {
    return m_quotes.size();
}


