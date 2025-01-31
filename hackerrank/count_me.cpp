#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    int T;
    cin >> T;
    cin.ignore(); // Ignore the newline after reading T

    while (T--) {
        string S;
        getline(cin, S); // Read the entire line

        istringstream iss(S);
        vector<string> words;
        string word;
        while (iss >> word) {
            words.push_back(word);
        }

        unordered_map<string, int> freq;
        int max_count = 0;
        string result;

        for (const string& w : words) {
            freq[w]++;
            int current = freq[w];
            if (current > max_count) {
                max_count = current;
                result = w;
            }
        }

        cout << result << " " << max_count << endl;
    }

    return 0;
}
