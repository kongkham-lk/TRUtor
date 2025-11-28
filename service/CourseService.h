#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include "../model/Course.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class CourseService {
private:
    const std::string COURSE_FILE = "courses.txt";

    // Helper functions for file I/O
    std::vector<Course> loadCoursesFromFile() const;
    void saveCoursesToFile(const std::vector<Course>& courses) const;
    std::string generateNextId(const std::vector<Course>& courses) const;

public:
    CourseService() = default;

    void createCourse(const Course& course); // Will assign an ID and save
    Course getCourse(const std::string& id);
    void updateCourse(const Course& course);
    void deleteCourse(const std::string& id);
    std::vector<Course> getAllCourses();
};

#endif // COURSE_SERVICE_H