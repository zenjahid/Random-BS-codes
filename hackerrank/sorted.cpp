#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    int T; // Number of test cases
    cin >> T;

    while (T--) {
        int N; // Size of the list
        cin >> N;

        vector<int> A(N);
        for (int i = 0; i < N; ++i) {
            cin >> A[i];
        }

        // Use a set to remove duplicates and sort the elements
        set<int> unique_elements(A.begin(), A.end());

        // Convert the set back to a vector (optional, for formatting)
        vector<int> result(unique_elements.begin(), unique_elements.end());

        // Print the sorted and deduplicated list
        for (size_t i = 0; i < result.size(); ++i) {
            cout << result[i] << (i < result.size() - 1 ? " " : "\n");
        }
    }

    return 0;
}
