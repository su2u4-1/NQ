#include "../include/qlib.h"

enum State {
    int_,
    float_,
    char_0,
    char_1,
    string_,
    identifier_,
    keyword_,
    symbol_,
    // add: +, +=
    // sub: -, -=
    // mut: *, *=, */
    // div: /, /=, //, /*
    // mod: %, %=
    // gt : >, >=, >>, >>=
    // eq : =, ==
    // lt : <, <=, <<, <<=
    // not: !, !=
    // and: &, &=, &&
    // or : |, |=, ||
    // xor: ^, ^=
    comment_0,
    comment_1,
    void_
} state;

vector<shared_ptr<Token>> lexer(const vector<string>& source_code, const fs::path& file_name) {
    vector<shared_ptr<Token>> tokens;
    string content = "";
    char pc = ' ';  // Previous character
    int s = 0;      // Symbol state
    pair<int, int> pos = make_pair(-1, -1);
    for (int i = 0; i < source_code.size(); i++) {
        string line = source_code[i];
        for (int j = 0; j < line.size(); j++) {
            char c = line[j];  // Current character

            // deal with state
            if (state == int_) {
                if ('0' <= c && c <= '9') {
                    content += c;
                    continue;
                } else if (c == '.') {
                    content += c;
                    state = float_;
                    continue;
                } else {
                    tokens.push_back(make_shared<Token>("int", content, file_name, pos));
                    content = "";
                    state = void_;
                }
            } else if (state == float_) {
                if ('0' <= c && c <= '9') {
                    content += c;
                    continue;
                } else {
                    tokens.push_back(make_shared<Token>("float", content, file_name, pos));
                    content = "";
                    state = void_;
                }
            } else if (state == identifier_) {
                if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || ('0' <= c && c <= '9')) {
                    content += c;
                    continue;
                } else {
                    if (is_keyword(content))
                        tokens.push_back(make_shared<Token>("keyword", content, file_name, pos));
                    else
                        tokens.push_back(make_shared<Token>("identifier", content, file_name, pos));
                }
            } else if (state == char_0) {
                if (c == '\'')
                    error("Character constant too short", file_name, {i + 1, j + 1}, line);
                else {
                    content += c;
                    state = char_1;
                }
                continue;
            } else if (state == char_1) {
                if (c != '\'')
                    error("Character constant too long", file_name, {i + 1, j + 1}, line);
                else {
                    tokens.push_back(make_shared<Token>("char", content, file_name, pos));
                    content = "";
                    state = void_;
                }
                continue;
            } else if (state == string_) {
                if (c == '"') {
                    tokens.push_back(make_shared<Token>("string", content, file_name, pos));
                    content = "";
                    state = void_;
                } else
                    content += c;
                continue;
            } else if (state == symbol_) {
                if ((s = is_symbol(content + c)) != 0) {
                    if (s == 1)
                        content += c;
                    else if (s == 2) {
                        tokens.push_back(make_shared<Token>("symbol", content + c, file_name, pos));
                        content = "";
                        state = void_;
                    } else if (s == 3) {
                        state = comment_0;
                        content += c;
                    } else if (s == 4) {
                        state = comment_1;
                        content += c;
                    }
                    continue;
                } else {
                    tokens.push_back(make_shared<Token>("symbol", content, file_name, pos));
                    content = "";
                    state = void_;
                }
            } else if (state == comment_0) {
                content += c;
                continue;
            } else if (state == comment_1) {
                content += c;
                if (pc == '*' && c == '/') {
                    tokens.push_back(make_shared<Token>("comment_1", content, file_name, pos));
                    content = "";
                    state = void_;
                }
                continue;
            }

            // deal with new character
            if ('0' <= c && c <= '9') {
                state = int_;
                content = c;
                pos = {i + 1, j + 1};
            } else if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_') {
                state = identifier_;
                content = c;
                pos = {i + 1, j + 1};
            } else if (c == '\'') {
                state = char_0;
                content = "";
                pos = {i + 1, j + 1};
            } else if (c == '"') {
                state = string_;
                content = "";
                pos = {i + 1, j + 1};
            } else if ((s = is_symbol(c)) != 0) {
                if (s == 1) {
                    state = symbol_;
                    content = c;
                    pos = {i + 1, j + 1};
                } else if (s == 2)
                    tokens.push_back(make_shared<Token>("symbol", string(c, 1), file_name, (i + 1, j + 1)));
            } else if (c == '#') {
                state = comment_0;
                content = "";
                pos = {i + 1, j + 1};
            }
            pc = c;
        }
        if (state == comment_0) {
            tokens.push_back(make_shared<Token>("comment_0", content, file_name, pos));
            state = void_;
            content = "";
        }
    }
    return tokens;
}
