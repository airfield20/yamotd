//
// Created by aaron on 2020-10-01.
//

#ifndef P1_QUOTEDB_H
#define P1_QUOTEDB_H

#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>


namespace cis427 {
    class QuoteDB {
    public:
        explicit QuoteDB(const std::string &path_to_quotes);

        QuoteDB();

        std::string get_random_quote();

        std::string get_quote(const unsigned long &index);

        int get_num_quotes();

    private:

        std::vector<std::string> m_quotes;
        std::ifstream m_file;

    };
}


#endif //P1_QUOTEDB_H
