#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Define a structure to represent a pair of string and integer
struct Pair {
    string S;
    int I;
};

int main() {
    int N; // Size of the list A
    cin >> N;

    vector<Pair> A(N);

    // Read the input pairs
    for (int i = 0; i < N; ++i) {
        cin >> A[i].S >> A[i].I;
    }

    // Sort the pairs based on the given criteria:
    // 1. String in ascending order
    // 2. Integer in descending order if strings are equal
    sort(A.begin(), A.end(), [](const Pair& a, const Pair& b) {
        if (a.S == b.S) {
            return a.I > b.I; // Descending order for integers
        }
        return a.S < b.S; // Ascending order for strings
    });

    // Print the sorted pairs
    for (const auto& p : A) {
        cout << p.S << " " << p.I << "\n";
    }

    return 0;
}
