#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

// Define a structure for a student
struct Student {
    string name;
    int roll;
    int marks;

    // Constructor
    Student(string n, int r, int m) : name(n), roll(r), marks(m) {}

    // Comparator for max-heap
    bool operator<(const Student& other) const {
        if (marks != other.marks) {
            return marks < other.marks; // Higher marks come first
        }
        return roll > other.roll; // Smaller roll comes first
    }
};

int main() {
    int N;
    cin >> N;

    // Max-heap to maintain the students
    priority_queue<Student> maxHeap;

    // Read the initial list of students
    for (int i = 0; i < N; ++i) {
        string name;
        int roll, marks;
        cin >> name >> roll >> marks;
        maxHeap.push(Student(name, roll, marks));
    }

    int Q;
    cin >> Q;

    while (Q--) {
        int command;
        cin >> command;

        if (command == 0) {
            // Insert X into the list
            string name;
            int roll, marks;
            cin >> name >> roll >> marks;
            maxHeap.push(Student(name, roll, marks));

            // Print the current student with maximum marks
            if (!maxHeap.empty()) {
                const Student& topStudent = maxHeap.top();
                cout << topStudent.name << " " << topStudent.roll << " " << topStudent.marks << "\n";
            } else {
                cout << "Empty\n";
            }
        } else if (command == 1) {
            // Print the current student with maximum marks
            if (!maxHeap.empty()) {
                const Student& topStudent = maxHeap.top();
                cout << topStudent.name << " " << topStudent.roll << " " << topStudent.marks << "\n";
            } else {
                cout << "Empty\n";
            }
        } else if (command == 2) {
            // Delete the student with maximum marks
            if (!maxHeap.empty()) {
                maxHeap.pop();
                if (!maxHeap.empty()) {
                    const Student& topStudent = maxHeap.top();
                    cout << topStudent.name << " " << topStudent.roll << " " << topStudent.marks << "\n";
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
