#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include "../model/Course.h"
#include <string>
#include <vector>

class CourseService {
public:
    CourseService() = default;

    // These will eventually connect to Firebase
    void createCourse(const Course& course);
    Course getCourse(const std::string& id);
    void updateCourse(const Course& course);
    void deleteCourse(const std::string& id);
    std::vector<Course> getAllCourses();

private:
    // Placeholder for future Firebase integration
    void connectToFirebase();
};

#endif // COURSE_SERVICE_H

