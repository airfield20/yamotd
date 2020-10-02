#include "../include/external/catch2/catch.hpp"
#include "../include/server/quotedb.h"
#include <iostream>

TEST_CASE("DB read test", "[db read]"){
    cis427::QuoteDB quote_db("../tests/test_quotes.txt");
    std::string quote = quote_db.get_random_quote();
    SECTION("DB contents") {
        REQUIRE(!quote.empty());
    }
    SECTION("read in amount"){
        REQUIRE(quote_db.get_num_quotes() == 4);
    }

}

TEST_CASE("DB Error test", "[error]"){
    REQUIRE_THROWS(cis427::QuoteDB("nofile.txt"));
}