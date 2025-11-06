#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <chrono>

using namespace std;

class HashTable {
private:
    static const int SIZE = 10007;

    struct Entry {
        string key;
        int value;
    };

    vector<list<Entry>> table;

    //dbj2 hash function

    int hash(string key) {
        long h = 5381;
        for (char c : key) {
            h = ((h << 5) + h) + c;
        }
        int index = (int)(h % SIZE);
        if (index < 0) index += SIZE;
        return index;
    }

public:
    HashTable() {
        table.resize(SIZE);
    }
    //seperate chaining
    void insert(string key, int value) {
        int index = hash(key);
        for (auto &entry : table[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }
        table[index].push_back({key, value});
    }

    bool get(string key, int &value) {
        int index = hash(key);
        for (auto &entry : table[index]) {
            if (entry.key == key) {
                value = entry.value;
                return true;
            }
        }
        return false;
    }
};

int main() {
    HashTable games;
    map<string, int> games2;
    ifstream file("all_data.csv");

    if (!file.is_open()) {
        cout << "Error opening csv" << endl;
        return 1;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string name, ratingStr;

        if (getline(ss, name, ',') && getline(ss, ratingStr, ',')) {
            if (ratingStr.empty()) continue;
            try {
                int rating = stoi(ratingStr);
                games.insert(name, rating);
                games2.insert(make_pair(name, rating));
            } catch (...) {}
        }
    }
    file.close();

    cout << "Welcome to\n-----------------\nSteam Game Finder\n-----------------" << endl;

    string search;
    while (true) {
        cout << "\nEnter a game name to search (or 'exit' to quit): ";
        getline(cin, search);
        if (search == "exit") break;

        int rating;
        auto start = chrono::high_resolution_clock::now();
        bool found = games.get(search, rating);
        auto end = chrono::high_resolution_clock::now();

        auto time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        if (found) {
            cout << search << " has " << rating << " positive ratings." << endl;
        } else {
            cout << "Game not found." << endl;
        }

        cout << "Search took " << time << " ns." << endl;

        //MAP SEARCH
        cout << "\nUsing Map/Red-Black Tree:" << endl;

        start = chrono::high_resolution_clock::now();
        auto map_found = games2.find(search);
        end = chrono::high_resolution_clock::now();

        time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        if (map_found !=games2.end())
        {
            cout << search << " has " << map_found->second << " positive ratings." << endl;
        }
        else
            cout << "Game not found." << endl;

        cout << "Search took " << time << " ns." << endl;
    }

    return 0;
}
