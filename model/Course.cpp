#include "Course.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

Course::Course(const std::string& id, const std::string& title, const std::string& description)
    : id_(id), title_(title), description_(description) {}

const std::string& Course::getId() const { return id_; }
const std::string& Course::getTitle() const { return title_; }
const std::string& Course::getDescription() const { return description_; }

void Course::setId(const std::string& id) { id_ = id; }
void Course::setTitle(const std::string& title) { title_ = title; }
void Course::setDescription(const std::string& description) { description_ = description; }

//Write course data to a stream
std::ostream& operator<<(std::ostream& os, const Course& course) {
    os << course.id_ << "|" << course.title_ << "|" << course.description_;
    return os;
}

//Read course data from a stream 
std::istream& operator>>(std::istream& is, Course& course) {
    std::string line;
    if (std::getline(is, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> parts;

        //Split based on the '|' delimiter
        size_t start = 0;
        size_t end = line.find('|');
        while (end != std::string::npos) {
            parts.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find('|', start);
        }
        parts.push_back(line.substr(start));

        if (parts.size() == 3) {
            course.id_ = parts[0];
            course.title_ = parts[1];
            course.description_ = parts[2];
        }
        else {
            is.setstate(std::ios::failbit);
            std::cerr << "Course::operator>> - Failed to parse line: " << line << std::endl;
        }
    }
    return is;
}