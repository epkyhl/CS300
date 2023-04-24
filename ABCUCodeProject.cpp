//============================================================================
// Name        : Easton Kyhl
// Class       : CS300
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


class Course {

private:
    string id;
    string name;
    vector<string> prereqs;
    int numPrereqs;

public:
    Course();
    Course(string courseId, string courseName, vector <string> coursePrereqs, int numPrereq);
    virtual ~Course();

    string getId();
    string getName();
    vector<string> getPrereqs();
    int getNumPrereqs();

    void setId(string id);
    void setName(string name);
    void setPrereqs(vector<string> prereqs);
    void setNumPrereqs(int numPrereqs);
    void addPrereqs(string prereq);
};

Course::Course() {
    id = "";
    name = "";
    vector<string> prereqs;
    numPrereqs = 0;
}

Course::Course(string courseId, string courseName, vector <string> coursePrereqs, int numPrereq) {
    id = courseId;
    name = courseName;
    prereqs = coursePrereqs;
    numPrereqs = numPrereq;
}

Course::~Course() {
    id = "";
    name = "";
    prereqs.resize(0);
    numPrereqs = 0;
}

string Course::getId() {
    return id;
}

string Course::getName() {
    return name;
}

vector<string> Course::getPrereqs() {
    return prereqs;
}

int Course::getNumPrereqs() {
    return numPrereqs;
}


void Course::setId(string courseId) {
    id = courseId;
}

void Course::setName(string courseName) {
    name = courseName;
}

void Course::setPrereqs(vector<string> coursePrereqs) {
    prereqs = coursePrereqs;
}

void Course::setNumPrereqs(int numprereq) {
    numPrereqs = numprereq;
}


vector<Course> loadCourses(string filePath) {
    Course currentCourse;
    fstream courseFS;
    vector<Course> courses;
    
    string courseTraits;
    
    courseFS.open(filePath);

    if (!courseFS.is_open()) {
        cout << "Could not open the file" << endl << endl;
        return courses;
    }

    while (getline(courseFS, courseTraits)) {
        stringstream courseSS(courseTraits);
        string trait;
        vector<string> courseInfo;

        while (courseSS.good()) {
            getline(courseSS, trait, ',');
            courseInfo.push_back(trait);
        }

        currentCourse.setId(courseInfo.at(0));
        currentCourse.setName(courseInfo.at(1));

        if (courseInfo.size() > 2) {
            vector<string> prereqs;

            for (int i = 2; i < courseInfo.size(); i++) {
                prereqs.push_back(courseInfo.at(i));
            }

            currentCourse.setPrereqs(prereqs);
            currentCourse.setNumPrereqs(courseInfo.size() - 2);
        }

        courses.push_back(currentCourse);
    }

    courseFS.close();
    return courses;
}

/**
 * Partition the vector of courses into two parts, low and high
 *
 * @param courses Address of the vector<Course> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Course>& courses, int begin, int end) {
    int low = begin;
    int high = end;
    int pivot;
    bool done = false;

    //Gets the pivot point for the quicksort algorithm
    pivot = low + (high - low) / 2;

    while (!done) {

        //Compares low and high to the pivot and increments high while there are values above/below the pivot point
        while (courses.at(low).getId() < courses.at(pivot).getId()) {
            low++;
        }

        while (courses.at(pivot).getId() < courses.at(high).getId()) {
            high--;
        }

        /* If there are zero or one elements remaining,
            all courses are partitioned. Return high */
        if (low >= high) {
            done = true;
        }

        //Swaps the low and high courses
        else {
            swap(courses.at(low), courses.at(high));

            //Brings low and high closer together
            low++;
            high--;
        }
    }
    return high;
}

/**
 * Perform a quick sort on Course Id
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param courses address of the vector<Course> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Course>& courses, int begin, int end) {
    int mid = 0;

    //Base Cases to stop calling recursively 
    if (courses.size() <= 1) {
        return;
    }

    if (begin >= end) {
        return;
    }

    /* Partitions courses into low and high such that
     midpoint is location of last element in low */
    mid = partition(courses, begin, end);

    //Recursively sorts low partition (begin to mid)
    quickSort(courses, begin, mid);

    //Recursively sorts high partition (mid+1 to end)
    quickSort(courses, mid + 1, end);
}

void printCourseInfo(Course currentCourse) {
    cout << currentCourse.getId() << " " << currentCourse.getName() << endl;

    if (currentCourse.getNumPrereqs() > 0) {
        cout << "This course has " << currentCourse.getNumPrereqs() << " prerequisite/s: " << endl;

        for (int i = 0; i < currentCourse.getNumPrereqs(); i++) {
            cout << currentCourse.getPrereqs().at(i) << endl;
        }

        cout << endl;
    }
}

void printAllCourses(vector<Course> courses) {
    quickSort(courses, 0, courses.size() - 1);

    for (int i = 0; i < courses.size(); i++) {
        printCourseInfo(courses.at(i));
    }
}

int main() {
    string userFilePath;
    string userCourseStr;
    vector<Course> courses;
    int userChoice = 0;

    while (userChoice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Print Select Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> userChoice;

        switch (userChoice) {

        case 1:
            cout << "What is the file name of the courses? " << endl;
            cin >> userFilePath;

            courses = loadCourses(userFilePath);

            if (courses.size() == 0) {
                break;
            }

            cout << "Courses loaded " << endl;

            break;

        case 2:
            printAllCourses(courses);

            break;

        case 3:
            cout << "What course would you like to display? " << endl;
            cin >> userCourseStr;

            for (int i = 0; i < courses.size(); i++) {
                if (userCourseStr == courses.at(i).getId()) {
                    printCourseInfo(courses.at(i));
                    break;
                }
            }

            break;
        }
    }

    cout << "Program Terminated. " << endl;

    return 0;
}

