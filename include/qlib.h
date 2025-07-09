#ifndef QLIB_H
#define QLIB_H

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;

class Token;
class Tokens;

void error(const string& msg, const fs::path& file_name, pair<int, int> pos, const string& source_code);
int is_symbol(const string& word);
int is_symbol(char c);
bool is_keyword(const string& word);

class Token {
   public:
    Token(const string& type, const string& value, const fs::path& file_name, pair<int, int> pos);
    Token(const string& type, const string& value);
    Token();
    string type;
    string value;
    fs::path file_name;
    pair<int, int> pos;
    string toString() const;
    bool operator==(const Token& other) const;
    bool operator==(const Tokens& other) const;
    bool operator!=(const Token& other) const;
    bool operator!=(const Tokens& other) const;
};

class Tokens {
   public:
    Tokens(const string& type, const vector<string>& value);
    string type;
    vector<string> value;
    string toString() const;
    bool operator==(const Token& other) const;
    bool operator!=(const Token& other) const;
};

#endif  // QLIB_H