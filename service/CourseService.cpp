#include "CourseService.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

vector<Course> CourseService::loadCoursesFromFile() const {
    vector<Course> courses;
    ifstream file(COURSE_FILE);
    if (!file.is_open()) return courses;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Course course;
        if (ss >> course) {
            courses.push_back(course);
        }
    }
    return courses;
}

void CourseService::saveCoursesToFile(const vector<Course>& courses) const {
    ofstream file(COURSE_FILE, ios::trunc);
    if (!file.is_open()) {
        cerr << "[CourseService] Error: Could not open " << COURSE_FILE << " for writing.\n";
        return;
    }
    for (const auto& course : courses) {
        file << course << "\n"; 
    }
}

string CourseService::generateNextId(const vector<Course>& courses) const {
    int maxNum = 0;
    for (const auto& course : courses) {
        try {
            int num = stoi(course.getId());
            if (num > maxNum) maxNum = num;
        }
        catch (...) {}
    }
    return to_string(maxNum + 1);
}

void CourseService::createCourse(const Course& course) {
    vector<Course> courses = loadCoursesFromFile();
    //Assign a new ID
    Course newCourse = course;
    newCourse.setId(generateNextId(courses));

    courses.push_back(newCourse);
    saveCoursesToFile(courses);
    cout << "[CourseService] Created course: " << newCourse.getTitle() << " (ID: " << newCourse.getId() << ")\n";
}

Course CourseService::getCourse(const std::string& id) {
    vector<Course> courses = loadCoursesFromFile();
    for (const auto& course : courses) {
        if (course.getId() == id) {
            return course;
        }
    }
    throw runtime_error("Course not found for ID: " + id);
}

void CourseService::updateCourse(const Course& course) {
    vector<Course> courses = loadCoursesFromFile();
    bool found = false;
    for (auto& c : courses) {
        if (c.getId() == course.getId()) {
            c = course;
            found = true;
            break;
        }
    }
    if (found) {
        saveCoursesToFile(courses);
        cout << "[CourseService] Updated course: " << course.getTitle() << " (ID: " << course.getId() << ")\n";
    }
    else {
        throw runtime_error("Cannot update: Course not found for ID: " + course.getId());
    }
}

void CourseService::deleteCourse(const std::string& id) {
    vector<Course> courses = loadCoursesFromFile();
    auto initialSize = courses.size();

    // Remove the course with the matching ID
    courses.erase(
        remove_if(courses.begin(), courses.end(),
            [&id](const Course& c) { return c.getId() == id; }),
        courses.end()
    );

    if (courses.size() < initialSize) {
        saveCoursesToFile(courses);
        cout << "[CourseService] Deleted course with ID: " << id << "\n";
    }
    else {
        throw runtime_error("Cannot delete: Course not found for ID: " + id);
    }
}

vector<Course> CourseService::getAllCourses() {
    cout << "[CourseService] Fetching all courses from file.\n";
    return loadCoursesFromFile();
}