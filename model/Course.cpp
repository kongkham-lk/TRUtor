#include "Course.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

Course::Course(const std::string& id, const std::string& code, const std::string& title, const std::string& description, int capacity, const std::string& tutorId, const std::string& day, const std::string& startTime, const std::string& endTime)
    : id_(id), code_(code), title_(title), description_(description), capacity_(capacity), tutorId_(tutorId), day_(day), startTime_(startTime), endTime_(endTime) {}

const std::string& Course::getId() const { return id_; }
const std::string& Course::getCode() const { return code_; }
const std::string& Course::getTitle() const { return title_; }
const std::string& Course::getDescription() const { return description_; }
const int& Course::getCapacity() const { return capacity_; }
const std::string& Course::getTutorId() const { return tutorId_; }
const std::string& Course::getDay() const { return day_; }
const std::string& Course::getStartTime() const { return startTime_; }
const std::string& Course::getEndTime() const { return endTime_; }

void Course::setId(const std::string& id) { id_ = id; }
void Course::setCode(const std::string& code) { code_ = code; }
void Course::setTitle(const std::string& title) { title_ = title; }
void Course::setDescription(const std::string& description) { description_ = description; }
void Course::setCapacity(const int& capacity) { capacity_ = capacity; }
void Course::setTutorId(const std::string& tutorId) { tutorId_ =
    tutorId; }
void Course::setDay(const std::string& day) { day_ = day; }
void Course::setStartTime(const std::string& startTime) { startTime_ = startTime; }
void Course::setEndTime(const std::string& endTime) { endTime_ = endTime; }

//Write course data to a stream
std::ostream& operator<<(std::ostream& os, const Course& course) {
    os << course.id_ << "|" << course.code_ << "|" << course.title_ << "|" 
    << course.description_ << "|" << course.capacity_ << "|" << course.tutorId_ << "|" 
    << course.day_ << "|" << course.startTime_ << "|" << course.endTime_;

    return os;
}

//Read course data from a stream 
std::istream& operator>>(std::istream& is, Course& course) {
    std::string line;
    if (!getline(is, line)) return is;

    stringstream ss(line);

    getline(ss, course.id_, '|');
    getline(ss, course.code_, '|');
    getline(ss, course.title_, '|');
    getline(ss, course.description_, '|');

    string cap;
    getline(ss, cap, '|');
    try {
    course.capacity_ = stoi(cap);
    } catch (...) {
    course.capacity_ = 0; // default or error handling
    // cerr << "[WARNING] Invalid capacity value: '" << cap << "'" << endl;
    }
    getline(ss, course.tutorId_, '|');
    getline(ss, course.day_, '|');
    getline(ss, course.startTime_, '|');
    getline(ss, course.endTime_, '|');
    return is;
}

