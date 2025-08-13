#include "../include/qlib.h"

unordered_set<string> _symbol1{"*", "/", "%", "+", "-", "<<", ">>", "<", ">", "&", "^", "|", "=", "!"};
unordered_set<string> _symbol2{"+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "<=", ">=", "==", "!=", "&&", "||", "(", ")", "[", "]", "{", "}", ",", ";", ":", ".", "~", "@", "$"};
unordered_set<string> _op2{"*", "/", "%", "+", "-", "<<", ">>", "<", "<=", ">", ">=", "==", "!=", "&", "^", "|", "&&", "||", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="};
unordered_set<string> _op1{"-", "!", "~", "@", "$"};
unordered_set<string> KEYWORD{"if", "elif", "else", "for", "in", "while", "return", "break", "continue", "import", "const", "public", "func", "method", "class", "var", "as", "op", "attr", "static", "true", "false", "void", "NULL"};
unordered_set<string> BUILTINTYPE{"int", "char", "bool", "void", "str", "float", "list", "pointer", "range", "type", "tuple", "dict"};
unordered_set<string> STDLIB{"io", "math"};

string HELP_DOCS =
    "Usage: qlib <filename> [options]\n"
    "Options:\n"
    "  -oa,  --output-ast        Output AST to file\n"
    "  -oaj, --output-ast-json   Output AST to JSON file\n"
    "  -oan, --output-ast-none   No output AST\n"
    "  -h,   --help              Show this help message\n";
fs::path BASEPATH = path_processing(fs::absolute(fs::path(__FILE__)).parent_path().parent_path());
fs::path STDLIBPATH = BASEPATH / "stdlib";
string VERSION = "x.y.z";  // Placeholder for version

void error(const string& msg, const fs::path& file_name, pair<int, int> pos) {
    error(msg, file_name, pos, source_code_set[file_name][pos.first - 1]);
}

void error(const string& msg, const fs::path& file_name, pair<int, int> pos, const string& source_code) {
    // File "d:\NQ\format.py", line 50, in <module>
    cerr << "file \"" << file_name << "\", line " << pos.first << "\n";
    cerr << source_code << "\n";
    cerr << string(pos.second - 1, ' ') << "^\n";
    cerr << "Error: " << msg << "\n";
    exit(EXIT_FAILURE);
}

int is_symbol(const string& word) {
    if (find(_symbol1.begin(), _symbol1.end(), word) != _symbol1.end())
        return 1;
    else if (find(_symbol2.begin(), _symbol2.end(), word) != _symbol2.end())
        return 2;
    else if (word == "//")
        return 3;
    else if (word == "/*")
        return 4;
    return 0;
}

int is_symbol(char c) {
    return is_symbol(string(1, c));
}

bool is_stdlib(const string& name) {
    return find(STDLIB.begin(), STDLIB.end(), name) != STDLIB.end();
}

bool is_keyword(const string& word) {
    return find(KEYWORD.begin(), KEYWORD.end(), word) != KEYWORD.end();
}

bool is_builtin_type(const string& word) {
    return find(BUILTINTYPE.begin(), BUILTINTYPE.end(), word) != BUILTINTYPE.end();
}

fs::path path_processing(const fs::path& file_path) {
    string file_name = file_path.string();
    if (PLATFORM_NAME == "windows") {
        if (file_name[0] == '/' && (file_name[1] > 'a' && file_name[1] < 'z')) {
            file_name = string(1, file_name[1] - 32) + ":" + file_name.substr(2);
        }
    }
    while (file_name.find("/./") != string::npos) {
        file_name.replace(file_name.find("/./"), 3, "/");
    }
    replace(file_name.begin(), file_name.end(), '\\', '/');
    return file_name;
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
