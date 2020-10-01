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

using std::string;

namespace cis427 {
    class quotedb {
    public:
        quotedb(const string &path_to_quotes);

        string get_random_quote();

    private:
        string get_quote(const unsigned long &index);

        std::vector<string> m_quotes;
        std::ifstream m_file;

    };
}


#endif //P1_QUOTEDB_H
