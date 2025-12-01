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

    while (true) {
        Course course;
        if (!(file >> course)) break;
        courses.push_back(course);
    }
    return courses;
}

void CourseService::saveCoursesToFile(const std::vector<Course>& courses) const {
    std::ofstream file(COURSE_FILE, std::ios::trunc);
    if (!file.is_open()) {
        cerr << "[CourseService] Error: Could not open " << COURSE_FILE << " for writing.\n";
        return;
    }
    for (const auto& course : courses) {
        file << course << "\n"; 
    }
}

vector<pair<string,string>> CourseService::loadEnrollments() const {
    vector<pair<string,string>> enrollments;
    std::ifstream file(ENROLL_FILE);
    if (!file.is_open()) return enrollments;
    string student, course;
    
    while (file >> student >> course)
        enrollments.push_back({ student, course });

    return enrollments;
}

void CourseService::saveEnrollments(const vector<pair<string,string>>& enrollments) const {
    ofstream file(ENROLL_FILE, ios::trunc);
    if (!file.is_open()) {
        cerr << "[CourseService] Error: Could not open " << ENROLL_FILE << " for writing.\n";
        return;
    }

    for (auto& stu : enrollments)
        file << stu.first << " " << stu.second << "\n";
}

vector<pair<string,string>> CourseService::loadWaitlist() const {
    vector<pair<string,string>> waitlist;
    ifstream file(WAITLIST_FILE);
    if (!file.is_open()) return waitlist;
    string student, course;

    while (file >> student >> course)
        waitlist.push_back({ student, course });

    return waitlist;
}

void CourseService::saveWaitlist(const vector<pair<string,string>>& waitlist) const {
    ofstream file(WAITLIST_FILE, ios::trunc);
    if (!file.is_open()) {
        cerr << "[CourseService] Error: Could not open " << WAITLIST_FILE << " for writing.\n";
        return;
    }

    for (auto& stu : waitlist)
        file << stu.first << " " << stu.second << "\n";
}

string CourseService::generateNextId(const vector<Course>& courses) const {
     int maxNum = 0;
    for (const Course& course : courses) {
        if (course.getId().size() > 1 && course.getId()[0] == 'C') {
            int idNum = stoi(course.getId().substr(1));
            maxNum = max(maxNum, idNum);
        }
    }
    return ("C" + to_string(maxNum + 1));
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

bool CourseService::deleteCourse(const string& tutorId, const string& courseId) {
    vector<Course> courses = loadCoursesFromFile();
    vector<pair<string,string>> enroll = loadEnrollments();

    // Block if any student enrolled already
    for (auto& stu : enroll) {
        if (stu.second == courseId) {
            cout << "Cannot delete course. Enrolled students exist.\n";
            return false;
        }
    }

    // Tutor can delete only their own courses
    auto iterator = remove_if(courses.begin(), courses.end(),
        [&](const Course& c) { return c.getId() == courseId && c.getTutorId() == tutorId; });

    if (iterator == courses.end()) {
        cout << "Course not found or you are not the owner.\n";
        return false;
    }

    courses.erase(iterator, courses.end());
    saveCoursesToFile(courses);
    return true;
}

//returns all the courses
vector<Course> CourseService::getAllCourses() {
    cout << "[CourseService] Fetching all courses from file.\n";
    return loadCoursesFromFile();  //get all courses
}

//get all courses of a particular tutor
vector<Course> CourseService::getCoursesByTutor(const string& tutorId) {
    vector<Course> coursesAll = loadCoursesFromFile();  //get all courses
    vector<Course> coursesTutor;   //store courses of the tutor

    for (auto& c : coursesAll)  //for each course
        if (c.getTutorId() == tutorId)  //check if tutorId matches
            coursesTutor.push_back(c);     //store course

    return coursesTutor;
}

//get all enrolled courses of a particular student
vector<Course> CourseService::getCoursesByStudent(const string& studentId) {
    vector<pair<string,string>> enroll = loadEnrollments();  //get all enrollment entries
    vector<Course> coursesAll = loadCoursesFromFile();  //get all courses   
    vector<Course> coursesStudent;  //store courses in which student is enrolled

    for (auto& e : enroll)  //for each enrollment entry
        if (e.first == studentId)  //check if studentId matches
            for (auto& c : coursesAll)  //for each course
                if (c.getId() == e.second)  //check if courseId matches
                    coursesStudent.push_back(c);       //store course

    return coursesStudent;
}

//get all courses which have waitlisted students
vector<Course> CourseService::getWaitlistedCourses(const string& studentId) {
    vector<pair<string,string>> waitlist = loadWaitlist();  //get all waitlisted entries
    vector<Course> coursesAll = loadCoursesFromFile();  //get all courses
    vector<Course> coursesWaitlisted;  //store coourses which have waitlisted students

    for (auto& w : waitlist)   //for each waitlist entry
        if (w.first == studentId)  //check if studentId matches
            for (auto& c : coursesAll)  //for each course
                if (c.getId() == w.second)  //check if courseId matches
                    coursesWaitlisted.push_back(c);         //store course
    return coursesWaitlisted; 
}

//enroll a student in a course
bool CourseService::enrollStudent(const string& studentId, const string& courseId) {
    vector<Course> courses = loadCoursesFromFile();  //get all courses
    vector<pair<string,string>> enroll = loadEnrollments();  //get all enrollment entries

    Course target;  //course to enroll in
    bool found = false;  //flag to check if course found
    for (auto& c : courses)  //for each course
        if (c.getId() == courseId) {  //check if courseId matches
            target = c;  //store course to enroll in
            found = true;  //update flag to true
        }

    if (!found) return false;  //course not found

    // Count enrolled
    int count = 0;
    for (auto& e : enroll)  //for each enrollment entry
        if (e.second == courseId)  //check if courseId matches
            count++;  

    if (count >= target.getCapacity()) return false; // incase max capacity of course already reached

    enroll.push_back({ studentId, courseId }); //add student to enrollment list
    saveEnrollments(enroll);  //save updated enrollment list

    return true;
}

bool CourseService::removeStudent(const string& studentId, const string& courseId) {
    //as there is a pair studentId, courseId in enrollment file for each enrollment
    vector<pair<string,string>> enroll = loadEnrollments();  //get all enrollment entries

    //iterator to find the enrollment entry
    auto iterator = remove_if(enroll.begin(), enroll.end(),
        [&](auto& enroledStudent) { return enroledStudent.first == studentId && enroledStudent.second == courseId; });  //if student matches the list

    if (iterator == enroll.end()) return false;  //student not found in enrollment list

    enroll.erase(iterator, enroll.end());  //remove student from enrollment list
    saveEnrollments(enroll);  //save updated enrollment list

    promoteWaitlistedStudents(courseId);  //promote waitlisted students if any
    return true;
}

bool CourseService::joinWaitlist(const string& studentId, const string& courseId) {
    auto waitlist = loadWaitlist();  //get all waitlisted entries
    waitlist.push_back({ studentId, courseId });  //add a student to waitlist
    saveWaitlist(waitlist);  //save updated waitlist
    return true;
}

//Students wanting to leave waitlist
bool CourseService::leaveWaitlist(const string& studentId, const string& courseId) {
    auto waitlist = loadWaitlist();  //get all waitlisted entries

    //iterater to find the waitlist entry
    auto iterator = remove_if(waitlist.begin(), waitlist.end(),
        [&](auto& student) { return student.first == studentId && student.second == courseId; }); //if student matches the list

    if (iterator == waitlist.end()) return false;  //student not found in waitlist

    waitlist.erase(iterator, waitlist.end());  //remove student from waitlist
    saveWaitlist(waitlist);  //save updated waitlist
    return true;
}

int CourseService::getWaitlistCount(const string& courseId) {
    auto waitlist = loadWaitlist();  //get all waitlisted entries
    int waitlistCount = 0;
    for (auto& w : waitlist)  //for each waitlist entry
        if (w.second == courseId) //check if courseId matches
            waitlistCount++;

    return waitlistCount;
}

//Promote waitlisted students to enrolled if seats available
void CourseService::promoteWaitlistedStudents(const string& courseId) {
    vector<Course> coursesAll = loadCoursesFromFile();  //get all courses
    auto waitlist = loadWaitlist();  //get all waitlisted entries   
    auto enroll = loadEnrollments();  //get all enrollment entries

    int capacity = 0;  //capacity of the course
    for (auto& c : coursesAll)  //for each course
        if (c.getId() == courseId)  //check if courseId matches
            capacity = c.getCapacity();  //store capacity of required course

    int enrolled = 0;  //number of students enrolled in the course
    for (auto& e : enroll)  //for each enrollment entry
        if (e.second == courseId)  //check if courseId matches with required course
            enrolled++;

    while (enrolled < capacity) {  //while seats are available
        bool promoted = false;  //flag to check for waitlisted student promoted

        //go through waitlist to find a student to promote
        for (auto iterator = waitlist.begin(); iterator != waitlist.end(); ++iterator)  //
            {
                if (iterator->second == courseId) {
                    enroll.push_back({ iterator->first, courseId });  //add student to enrollment list
                    waitlist.erase(iterator);  //remove student from waitlist

                    promoted = true;  //update flag to true(as student got promoted)
                    enrolled++; //increment enrolled count
                    break;
                }
            }
        if (!promoted) break; //cannot promote any more studentt as capacity reached or no waitlisted students
    }

    saveEnrollments(enroll);  //save updated enrollment list
    saveWaitlist(waitlist);  //save updated waitlist
}

int CourseService::countEnrolled(const string& courseId) const {
    ifstream file(ENROLL_FILE);
    if (!file.is_open()) return 0;

    string line;
    int count = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string studentId, cId;

        getline(ss, studentId, '|');
        getline(ss, cId, '|');

        if (cId == courseId)
            count++;
    }

    return count;
}


//display all the courses
void CourseService::listAllCourses() {
    vector<Course> coursesAll = loadCoursesFromFile();  //get all courses

    if (coursesAll.empty()) {
        cout << "No courses available.\n";
        return;
    }
    for (auto& c : coursesAll) 
    {  //for each course
        if (c.getId().empty()) {cout<<"No courses available.\n"; continue;}
        cout << "Course ID : " << c.getId() << " \n "
             << "Tutor ID : " << c.getTutorId() << " \n "
             << "Course Code : " << c.getCode() << " \n "
             << "Title : " << c.getTitle() << " \n" 
             << "Description : " << c.getDescription() << " \n"
             << "Max Capacity:" << c.getCapacity() << " | " << "Enrolled : " << countEnrolled(c.getId()) <<"Spots Remaining : " << (c.getCapacity() - countEnrolled(c.getId())) << " \n"
             << "Waitlist:" << getWaitlistCount(c.getId()) << " \n"
             << "Schedule : " << c.getDay() << " " << c.getStartTime() << "-" << c.getEndTime() << "\n"
             << "----------------------------------------------------------------------\n"
             << "\n";
             
    }
}

//display all courses of a particular tutor
void CourseService::listTutorCourses(const string& tutorId) {
    if (getCoursesByTutor(tutorId).empty()) {
        cout << "No courses available for this tutor.\n";
        return;
    }

    for (auto& c : getCoursesByTutor(tutorId)) 
    { //for each course
        cout << "Course ID : " << c.getId() << " \n "
             << "Tutor ID : " << c.getTutorId() << " \n "
             << "Course Code : " << c.getCode() << " \n "
             << "Title : " << c.getTitle() << " \n" 
             << "Description : " << c.getDescription() << " \n"
             << "Max Capacity:" << c.getCapacity() << " | " << "Enrolled : " << countEnrolled(c.getId()) <<"Spots Remaining : " << (c.getCapacity() - countEnrolled(c.getId())) << " \n"
             << "Waitlist:" << getWaitlistCount(c.getId()) << " \n"
             << "Schedule : " << c.getDay() << " " << c.getStartTime() << "-" << c.getEndTime() << "\n"
             << "----------------------------------------------------------------------\n"
             << "\n";
    }
}

//display all enrolled courses of a particular student
void CourseService::listStudentCourses(const string& studentId) {
    if (getCoursesByStudent(studentId).empty()) {
        cout << "No enrolled courses for this student.\n";
        return;
    }
    for (auto& c : getCoursesByStudent(studentId)) 
    { //for each course
        cout << "Course ID : " << c.getId() << " \n "
             << "Tutor ID : " << c.getTutorId() << " \n "
             << "Course Code : " << c.getCode() << " \n "
             << "Title : " << c.getTitle() << " \n" 
             << "Description : " << c.getDescription() << " \n"
             << "Max Capacity:" << c.getCapacity() << " | " << "Enrolled : " << countEnrolled(c.getId()) <<"Spots Remaining : " << (c.getCapacity() - countEnrolled(c.getId())) << " \n"
             << "Waitlist:" << getWaitlistCount(c.getId()) << " \n"
             << "Schedule : " << c.getDay() << " " << c.getStartTime() << "-" << c.getEndTime() << "\n"
             << "----------------------------------------------------------------------\n"
             << "\n";
    }
}

//display all waitlisted courses of a particular student
void CourseService::listStudentWaitlisted(const string& studentId) {
    if (getWaitlistedCourses(studentId).empty()) {
        cout << "No waitlisted courses for this student.\n";
        return;
    }
    for (auto& c : getWaitlistedCourses(studentId)) 
    { //for each course
        cout << "WAITLISTED COURSE DETAILS\n";
        cout << "----------------------------------------------------------------------\n";
        cout << "Course ID : " << c.getId() << " \n "
             << "Tutor ID : " << c.getTutorId() << " \n "
             << "Course Code : " << c.getCode() << " \n "
             << "Title : " << c.getTitle() << " \n" 
             << "Description : " << c.getDescription() << " \n"
             << "Max Capacity:" << c.getCapacity() << " | " << "Enrolled : " << countEnrolled(c.getId()) <<"Spots Remaining : " << (c.getCapacity() - countEnrolled(c.getId())) << " \n"
             << "Waitlist:" << getWaitlistCount(c.getId()) << " \n"
             << "Schedule : " << c.getDay() << " " << c.getStartTime() << "-" << c.getEndTime() << "\n"
             << "----------------------------------------------------------------------\n"
             << "\n";
    }
}

