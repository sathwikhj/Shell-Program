#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

// Function prototypes
void touch(const vector<string> &params, vector<string> &history);
void ls(const vector<string> &params, vector<string> &history);
void cd(const vector<string> &params, vector<string> &history);
void copy(const vector<string> &params, vector<string> &history);
void pwd(const vector<string> &params, vector<string> &history);
void showHistory(const vector<string> &params, vector<string> &history);
void clearHistory(const vector<string> &params, vector<string> &history);

// Map command names to corresponding functions
unordered_map<string, void (*)(const vector<string> &, vector<string> &)> commandMap = {
    {"touch", touch},
    {"ls", ls},
    {"cd", cd},
    {"copy", copy},
    {"pwd", pwd},
    {"history", showHistory},
    {"clear", clearHistory}
};

// Command implementations
void touch(const vector<string> &params, vector<string> &history) {
    if (params.empty()){
        cout << "How to Use?: touch <filenames>..." << endl;
        return;
    }
    for(const string& filename : params){
        ofstream file(filename);
        file.close();
        cout << "File created: " << filename << endl;
        history.push_back("touch " + filename);
    }
}


void ls(const vector<string> &params, vector<string> &history) {
    fs::path currentPath = fs::current_path();
    for(const auto &entry : fs::directory_iterator(currentPath)){
        cout << entry.path().filename() << endl;
    }
    history.push_back("ls");
}

void cd(const vector<string> &params, vector<string> &history){
    if(params.empty()){
        cout << "How to Use?: cd <directory>" << endl;
        return;
    }
    fs::path newPath = fs::absolute(params[0]);
    if(fs::exists(newPath) && fs::is_directory(newPath)){
        fs::current_path(newPath);
        cout << "Currently In: " << fs::current_path() << endl;
    } 
    else{
        cout << "Directory not found: " << params[0] << endl;
    }
    history.push_back("cd");
}

void copy(const vector<string> &params, vector<string> &history){
    if(params.size() < 2){
        cout << "How to Use?: copy <source> <destination>" << endl;
        return;
    }
    if (!fs::exists(params[0])){
        cout << "Source file not found: " << params[0] << endl;
        return;
    }
    if (params[0] == params[1]) {
        cout << "Error: Source and destination files are the same" << endl;
        return;
    }
    fs::copy(params[0], params[1], fs::copy_options::overwrite_existing);
    cout << "Copied: " << params[0] << " into " << params[1] << endl;
    history.push_back("copy "+params[0]+" "+params[1]);
}


void pwd(const vector<string> &params, vector<string> &history){
    cout << "Currently In: " << fs::current_path() << endl;
    history.push_back("pwd");
}

void showHistory(const vector<string> &params, vector<string> &history){
    for (const string &cmd : history)
        cout << cmd << endl;
}

void clearHistory(const vector<string> &params, vector<string> &history){
    history.clear();
    cout << "Command history cleared" << endl;
}

int main() {
    vector<string> history;
    string input;
    while(true){
        cout << "Sath_Shell> ";
        getline(cin, input);
        stringstream ss(input);
        string command;
        ss >> command;
        
        // Check if the user wants to exit
        if (command == "exit") {
            cout << "Bye :)" << endl;
            break;
        }        
        vector<string> params;
        string param;
        while (ss >> param){
            params.push_back(param);
        }
        auto it = commandMap.find(command);
        if(it != commandMap.end()){
            (it->second)(params, history);
        }     
        else{
            cout << "Command not found: " << command << endl;
        }
    }
    return 0;
}
