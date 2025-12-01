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
    const std::string ENROLL_FILE = "enrollments.txt";
    const std::string WAITLIST_FILE = "waitlist.txt";

    std::vector<Course> loadCoursesFromFile() const;
    void saveCoursesToFile(const std::vector<Course>& courses) const;
    std::vector<std::pair<std::string, std::string>> loadEnrollments() const;   //check later
    void saveEnrollments(const std::vector<std::pair<std::string, std::string>>& enrollments) const;  //check later
    std::vector<std::pair<std::string, std::string>> loadWaitlist() const;
    void saveWaitlist(const std::vector<std::pair<std::string, std::string>>& waitlist) const;
    std::string generateNextId(const std::vector<Course>& courses) const;

public:
    CourseService() = default;

    void createCourse(const Course& course);
    Course getCourse(const std::string& courseId);
    void updateCourse(const Course& courseUpdate);
    bool deleteCourse(const std::string& tutorId, const std::string& courseId);  //cannot delete if students enrolled

    bool enrollStudent(const std::string& studentId, const std::string& courseId);
    bool removeStudent(const std::string& studentId, const std::string& courseId);
    bool joinWaitlist(const std::string& studentId, const std::string& courseId);
    bool leaveWaitlist(const std::string& studentId, const std::string& courseId);

    int getWaitlistCount(const std::string& courseId);
    int countEnrolled(const std::string& courseId) const;

    void promoteWaitlistedStudents(const std::string& courseId);
    void listAllCourses();
    void listTutorCourses(const std::string& tutorId);
    void listStudentCourses(const std::string& studentId);
    void listStudentWaitlisted(const std::string& studentId);
    std::vector<Course> getAllCourses();
    std::vector<Course> getCoursesByTutor(const std::string& tutorId);
    std::vector<Course> getCoursesByStudent(const std::string& studentId);
    std::vector<Course> getWaitlistedCourses(const std::string& studentId);

    ~CourseService() {};
};

#endif // COURSE_SERVICE_H