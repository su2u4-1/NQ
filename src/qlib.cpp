#include "../include/qlib.h"

vector<string> symbol1{"*", "/", "%", "+", "-", "<<", ">>", "<", ">", "&", "^", "|", "=", "!"};
vector<string> symbol2{"+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "<=", ">=", "==", "!=", "&&", "||", "(", ")", "[", "]", "{", "}", ",", ";", ".", "~", "@", "$"};
vector<string> op2{"*", "/", "%", "+", "-", "<<", ">>", "<", "<=", ">", ">=", "==", "!=", "&", "^", "|", "&&", "||", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="};
vector<string> op1{"-", "!", "~", "@", "$"};
vector<string> keyword{"if", "elif", "else", "for", "in", "while", "return", "break", "continue", "import", "const", "public", "func", "method", "class", "var", "as", "true", "false", "void", "NULL"};

void error(const string& msg, const fs::path& file_name, pair<int, int> pos, const string& source_code) {
    // File "d:\NQ\format.py", line 50, in <module>
    cerr << "file \"" << file_name << "\", line " << pos.first << "\n";
    cerr << source_code << "\n";
    cerr << string(pos.second - 1, ' ') << "^\n";
    cerr << "Error: " << msg << "\n";
    exit(EXIT_FAILURE);
}

int is_symbol(const string& word) {
    for (const string& sym : symbol1)
        if (word == sym) return 1;
    for (const string& sym : symbol2)
        if (word == sym) return 2;
    if (word == "//") return 3;
    if (word == "/*") return 4;
    return 0;
}

int is_symbol(char c) {
    return is_symbol(string(1, c));
}

bool is_keyword(const string& word) {
    for (const string& kw : keyword)
        if (word == kw) return true;
    return false;
}

// class Token
Token::Token(const string& type, const string& value, const fs::path& file_name, pair<int, int> pos) {
    this->type = type;
    this->value = value;
    this->file_name = file_name;
    this->pos = pos;
}

Token::Token(const string& type, const string& value) {
    this->type = type;
    this->value = value;
    this->file_name = "";
    this->pos = make_pair(-1, -1);
}

Token::Token() {
    this->type = "";
    this->value = "";
    this->file_name = "";
    this->pos = make_pair(-1, -1);
}

string Token::toString() const {
    return "<" + type + ">(" + value + ", " + file_name.string() + ", (" + to_string(pos.first) + ", " + to_string(pos.second) + "))";
}

bool Token::operator==(const Token& other) const {
    return type == other.type && value == other.value;
}

bool Token::operator==(const Tokens& other) const {
    if (type != other.type) return false;
    for (const string& val : other.value)
        if (val == value) return true;
    return false;
}

bool Token::operator!=(const Token& other) const {
    return type != other.type || value != other.value;
}

bool Token::operator!=(const Tokens& other) const {
    if (type != other.type) return true;
    for (const string& val : other.value)
        if (val == value) return false;
    return true;
}

// class Tokens
Tokens::Tokens(const string& type, const vector<string>& value) {
    this->type = type;
    this->value = value;
}

string Tokens::toString() const {
    string result = "<" + type + ">(";
    for (const string& val : value)
        result += val + ", ";
    if (!value.empty()) {
        result.pop_back();
        result.pop_back();
    }
    result += ")";
    return result;
}

bool Tokens::operator==(const Token& other) const {
    if (type != other.type) return false;
    for (const string& val : value)
        if (val == other.value) return true;
    return false;
}

bool Tokens::operator!=(const Token& other) const {
    if (type != other.type) return true;
    for (const string& val : value)
        if (val == other.value) return false;
    return true;
}
