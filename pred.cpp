#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

namespace settings{
    int DEPTH = 5, LARGE_TEMP = 60, SMALL_TEMP = 80; //100 optimal/boring
} using namespace settings;

unordered_map<string,int> to_index;
map<vector<int>, map<int,int>> counter;
map<vector<int>, vector<pair<int,int>>> lookup;
vector<string> to_word = {""};

string train_data = " ";

int chance(char c){
    if(isalpha(c) || c == '\'') return 100;
    else if(c == '.') return 70;
    else if(c == '!') return 80;
    else if(c=='?') return 70;
    else if(c == ',') return 40;
    else if(c == ';') return 90;
    else if(c == ':') return 90;
    else return 0;
}

bool match(string &name, string &prefix){
    if(name.rfind(prefix,0) != 0) return false;
    if(name.size() == prefix.size()) return true;
    return !isdigit(name[prefix.size()]);
}

int allwords;
void prep_data(){
    int numfile = 1;
    char last_char = '\0';
    bool all = false;
    while(true){
        string folder = "text";
        string prefix = "file" + to_string(numfile);
        string filename="";

        for (const auto& entry : fs::directory_iterator(folder)) {
            string name = entry.path().filename().string();
            if (match(name, prefix)) {
                filename = folder + "/" + name;
                break;
            }
        }


        if(filename.empty()) break;
        ifstream file(filename);
        string title = filename.substr(folder.size()+prefix.size()+1);
        for(char &c: title) c = toupper(c);

        if(!all){
            cout << "INCLUDE FILE " << numfile << " \"" << ((title.empty())? "UNKOWN": title) << "\"" << "? (Y/N/ALL)  ";
            string s; cin >> s;
            for(char &c: s) c = toupper(c);
            if(s == "ALL"){all = true;}
            else if(s != "Y") {numfile++; continue;}
        }
        else cout << "FILE " << numfile << " \"" << ((title.empty())? "UNKOWN": title) << "\"" << " INCLUDED\n";

        string word;
        while(file >> word){
            for(char &c: word){
                int ch = chance(c);
                if(chance(last_char)<100 && ch < 100) continue;
                c = tolower(c);
                if(ch < 100){
                    if(rand()%100 < ch){
                        train_data += " ";
                        train_data += c;
                        train_data += " ";
                        allwords++;
                    }
                    else train_data += " ";
                }
                else train_data += c;
                last_char = c;
            }
            allwords += !isspace(train_data.back());
            train_data += " ";
        }

        file.close();
        numfile++;
    }
    cin.ignore();
    //cout << train_data;
}

void preparation(){
    srand(time(NULL));
    prep_data();
    cout << "PREPARING...\n";
    istringstream iss(train_data);
    string word; int numofwords = 1;
    int processed = 0, loading = 1;

    deque<int> analysed;
    while(iss >> word){
        processed++;
        if(processed*3 >= allwords*loading && loading <= 3) cout << (loading++) * 10 << "%\n";

        if(!to_index[word]) {to_index[word] = numofwords++; to_word.push_back(word);}
        analysed.push_back(to_index[word]);
        if(analysed.size() > DEPTH) analysed.pop_front();
        if(analysed.size() == 1) continue;

        for(int i=0;i<analysed.size()-1;i++){
            vector<int> v;
            for(int j=i;j<analysed.size()-1;j++){
                v.push_back(analysed[j]);
            }
            counter[v][analysed.back()]++;
        }
    }

    loading = 1; processed = 0;
    for(auto [key, mp]: counter){
        processed++;
        if(processed*7 >= counter.size()*loading) cout << (loading++) * 10 + 30 << "%\n";
        for(auto [small_key, result]: mp){
            lookup[key].push_back({result, small_key});
        }
        sort(lookup[key].begin(), lookup[key].end(), greater<pair<int,int>>());
    }
}

int main(){
    preparation();
    cout << "\n\nREADY\n" << "COMMANDS: \"set&\"\n";

    deque<int> analysed;
    bool sentences = false;
    while(1){
        //input
        string line,word; getline(cin, line);
        istringstream iss(line);
        while(iss >> word){
            for(char &c: word) c = tolower(c);
            if(word == "set&"){
                cout << "CURRENT RANDOMNESS (BOTH 0-100): " << 100-LARGE_TEMP << " " << 100-SMALL_TEMP << "\nSET: ";
                int a,b; cin >> a >> b;
                if(a<0 || a>100 || b<0 || b>100){cin.ignore(); continue;}
                LARGE_TEMP = 100-a; SMALL_TEMP = 100-b;
                cin.ignore();

                cout << "CURRENT MODE: " << (sentences? "SENTENCES": "WORDS") << "\nSET (W/S): ";
                char c; cin >> c;
                c = toupper(c);
                if(c=='W') sentences = false;
                else if(c=='S') sentences = true;
                cin.ignore();
                continue;
            }
            if(to_index[word]) analysed.push_back(to_index[word]);
            else analysed.clear();
        }

        //response
        while(analysed.size()>DEPTH) analysed.pop_front();
        do{
            bool responded = false;
            for(int i=0;i<analysed.size() && responded == false ;i++){
                vector<int> v(analysed.begin()+i, analysed.end());
                if(!lookup[v].empty() && (i==analysed.size()-1 || rand()%100 < LARGE_TEMP)){
                    int choice = 0;
                    for(choice;choice<lookup[v].size()-1;choice++){
                        int random = rand()%100;
                        if(random <= SMALL_TEMP) break;
                    }
                    int result = lookup[v][choice].second;
                    cout << to_word[result] << (sentences? " ": " | ");
                    analysed.push_back(result);
                    responded = true;
                }
            }
            if(!responded){
                int random_pick = rand()%to_word.size();
                cout << to_word[random_pick] << (sentences? " ": " / ");;
                analysed.push_back(random_pick);
            }
            while(analysed.size()>DEPTH) analysed.pop_front();
        }while(sentences && chance(to_word[analysed.back()][0]) == 100);
        if(sentences) cout << '\n';
    }
}
