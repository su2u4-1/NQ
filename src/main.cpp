#include "../include/lexer.h"
#include "../include/qlib.h"

struct arguments {
    fs::path program_name = fs::path("output");
    vector<fs::path> files = {};
    vector<string> flags = {};
    fs::path output_ast_file = fs::path("output.ast");
    int output_ast_type = -1;
    fs::path compile_file = fs::path("output.vm");
    bool compile = false;
    /*
    Do not output ast = -1
    The output file extension is .ast = 0
    The output file extension is _ast.json = 1
    */
};

vector<string> read_file(const fs::path& file_name) {
    ifstream input;
    input.open(file_name);
    if (input.fail())
        throw runtime_error("Could not open file " + file_name.string());
    vector<string> source_code;
    string line;
    while (getline(input, line)) {
        if (line.size() > 0) {
            if (line.back() == '\r')
                line.pop_back();
            if (line.back() != '\n')
                line.append("\n");
        }
        source_code.push_back(line);
    }
    input.close();
    return source_code;
}

arguments parse_arguments(int argc, char* argv[]) {
    arguments args;
    if (argc < 2) {
        cerr << "Usage: " << path_processing(argv[0]) << " <filename>" << endl;
        exit(1);
    }

    int state = 0;
    for (int i = 1; i < argc; i++) {
        string arg = string(argv[i]);
        if (arg[0] == '-') {
            args.flags.push_back(arg);
            if (arg == "--output-ast" || arg == "-oa") {
                args.output_ast_type = 0;
                args.output_ast_file = path_processing(args.program_name.replace_extension(".ast"));
            } else if (arg == "--output-ast-json" || arg == "-oaj") {
                args.output_ast_type = 1;
                args.output_ast_file = path_processing(args.program_name.replace_extension("_ast.json"));
            } else if (arg == "--output-ast-none" || arg == "-oan")
                args.output_ast_type = -1;
            else if (arg == "--help" || arg == "-h") {
                cout << HELP_DOCS;
                exit(0);
            } else if (arg == "--output" || arg == "-o")
                state = 1;
            else if (arg == "--stdlibpath" || arg == "-sp")
                state = 2;
            else if (arg == "--compile" || arg == "-c") {
                args.compile_file = path_processing(args.program_name.replace_extension(".vm"));
                args.compile = true;
            } else if (arg == "--version" || arg == "-v") {
                cout << "Version: " << VERSION << endl;
                exit(0);
            } else
                throw "Error: Unknown argument " + arg;
        } else {
            fs::path t = path_processing(fs::absolute(arg));
            if (state == 0) {
                if (fs::exists(t) && t.extension() == ".qn")
                    args.files.push_back(t);
                else {
                    cerr << "Error: File " << t << " does not exist or is not a .qn file" << endl;
                    exit(1);
                }
            } else if (state == 1) {
                args.program_name = path_processing(t.parent_path() / t.stem());
                state = 0;
            } else if (state == 2) {
                STDLIBPATH = t;
                state = 0;
            }
        }
    }
    return args;
}

string ast_to_json(shared_ptr<Node> node) {
    stringstream output;
    output << "{";
    output << "\"type\":\"" << node->type << "\",";
    output << "\"value\":{";
    for (const auto& [k, v] : node->value)
        output << '"' << k << "\":\"" << v << "\",";
    output << "},";
    output << "\"children\":[";
    for (const shared_ptr<Node>& child : node->children)
        output << ast_to_json(child);
    output << "]";
    output << "},";
    return output.str();
}

string remove_json_trailing_comma(const string& json) {
    char n, p;
    bool str = false;
    string result;
    for (int i = 0; i < json.size(); i++) {
        p = n;
        n = json[i];
        if (n == '"')
            str = !str;
        if (!str && p == ',' && (n == '}' || n == ']'))
            result[result.size() - 1] = n;
        else
            result += n;
    }
    if (result[result.size() - 1] == ',')
        result[result.size() - 1] = ' ';
    return result;
}

string output_ast(const shared_ptr<Node> node, int ident) {
    stringstream output;
    output << string(ident * 4, ' ') << node->type << " (";
    for (const auto& [k, v] : node->value)
        output << k << ": " << v << ", ";
    if (node->children.size() == 0)
        output << ") {}" << endl;
    else {
        output << ")" << endl;
        output << string(ident * 4, ' ') << "{" << endl;
        ident++;
        for (const auto& child : node->children)
            output << output_ast(child, ident);
        ident--;
        output << string(ident * 4, ' ') << "}" << endl;
    }
    return output.str();
}

int main(int argc, char* argv[]) {
    arguments args = parse_arguments(argc, argv);
    return 0;
}