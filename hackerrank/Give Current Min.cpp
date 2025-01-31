#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;

    // Read the initial list A
    vector<int> A(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
    }

    // Min-heap to maintain the elements
    priority_queue<int, vector<int>, greater<int>> minHeap;

    // Insert all elements of A into the min-heap
    for (int x : A) {
        minHeap.push(x);
    }

    int Q;
    cin >> Q;

    while (Q--) {
        int command;
        cin >> command;

        if (command == 0) {
            // Insert X into the list
            int X;
            cin >> X;
            minHeap.push(X);

            // Print the current minimum value
            cout << minHeap.top() << "\n";
        } else if (command == 1) {
            // Print the current minimum value
            if (!minHeap.empty()) {
                cout << minHeap.top() << "\n";
            } else {
                cout << "Empty\n";
            }
        } else if (command == 2) {
            // Delete the current minimum value
            if (!minHeap.empty()) {
                minHeap.pop();
                if (!minHeap.empty()) {
                    cout << minHeap.top() << "\n";
                } else {
                    cout << "Empty\n";
                }
            } else {
                cout << "Empty\n";
            }
        }
    }

    return 0;
}
