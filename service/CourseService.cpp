#include "CourseService.h"
#include <iostream>


void CourseService::connectToFirebase() {
    // Placeholder function
    std::cout << "[Firebase] Connection not yet implemented.\n";
}

void CourseService::createCourse(const Course& course) {
    connectToFirebase();
    std::cout << "[Firebase] Would create course: "
        << course.getTitle() << " (" << course.getId() << ")\n";
}

Course CourseService::getCourse(const std::string& id) {
    connectToFirebase();
    std::cout << "[Firebase] Would fetch course with ID: " << id << "\n";
    // Return dummy data for now
    return Course(id, "Sample Course", "This is a placeholder course until Firebase is connected.");
}

void CourseService::updateCourse(const Course& course) {
    connectToFirebase();
    std::cout << "[Firebase] Would update course: "
        << course.getTitle() << " (" << course.getId() << ")\n";
}

void CourseService::deleteCourse(const std::string& id) {
    connectToFirebase();
    std::cout << "[Firebase] Would delete course with ID: " << id << "\n";
}

std::vector<Course> CourseService::getAllCourses() {
    connectToFirebase();
    std::cout << "[Firebase] Would fetch all courses.\n";
    // Return dummy data for now
    return {
        Course("1", "Math Basics", "Introductory math concepts."),
        Course("2", "English Grammar", "Fundamentals of English grammar.")
    };
}
