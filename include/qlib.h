#ifndef QLIB_H
#define QLIB_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#if defined(_WIN32)
#define PLATFORM_NAME "windows"  // Windows
#elif defined(_WIN64)
#define PLATFORM_NAME "windows"  // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows"  // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__linux__)
#define PLATFORM_NAME "linux"
#endif

using namespace std;
namespace fs = filesystem;

class Token;
class Tokens;

extern string HELP_DOCS;
extern unordered_set<string> KEYWORD;
extern fs::path BASEPATH;
extern fs::path STDLIBPATH;
extern string VERSION;

void error(const string& msg, const fs::path& file_name, pair<int, int> pos, const string& source_code);
int is_symbol(const string& word);
int is_symbol(char c);
bool is_keyword(const string& word);
fs::path path_processing(const fs::path& file_path);

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

class Node {
   public:
    Node(const string& type, const map<string, string>& value, const vector<shared_ptr<Node>>& children, pair<int, int> pos);
    Node(const string& type, const map<string, string>& value, const shared_ptr<Node>& child, pair<int, int> pos);
    Node(const string& type, const map<string, string>& value, pair<int, int> pos);
    Node(const string& type, const map<string, string>& value);
    Node(const string& type, pair<int, int> pos);
    Node(const string& type);
    Node();
    string type;
    map<string, string> value;
    vector<shared_ptr<Node>> children;
    pair<int, int> pos;
    string toString() const;
};

#endif  // QLIB_H
