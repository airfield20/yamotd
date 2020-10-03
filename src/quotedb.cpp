//
// Created by aaron on 2020-10-01.
//

#include "../include/server/quotedb.h"

cis427::QuoteDB::QuoteDB(const std::string& path_to_quotes) {
    std::srand(std::time(NULL));
    m_path_to_quotes = path_to_quotes;
    m_file.open(path_to_quotes.c_str(), std::ios::in);
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
    m_file.close();
}

cis427::QuoteDB::QuoteDB() {}

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

bool cis427::QuoteDB::remove_last_quote() {
    m_quotes.pop_back();
    write_quotes_to_file();
    return true;
}

bool cis427::QuoteDB::add_quote(const std::string &quote, const std::string &author) {
    if(quote.empty() || author.empty()){
        return false;
    }

    std::string input;
    if(quote.at(quote.size() - 1) == '\n'){
        input = quote;
    }
    else{
        input = quote + '\n';
    }
    input = input + "-- " + author;
    m_quotes.push_back(input);
    write_quotes_to_file();
    return true;
}

bool cis427::QuoteDB::write_quotes_to_file() {
    m_file.open(m_path_to_quotes.c_str(), std::ios::out | std::ios::trunc);
    for(int i=0;i < m_quotes.size();i++){
        m_file << m_quotes.at(i) << std::endl << std::endl;
    }
    m_file.close();
    return false;
}


