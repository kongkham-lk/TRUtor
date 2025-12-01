#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


class Course {
public:
    Course() {}
    Course(const std::string& id, const std::string& code, const std::string& title, const std::string& description, int capacity, const std::string& tutorId, const std::string& day, const std::string& startTime, const std::string& endTime);

    const std::string& getId() const;
    const std::string& getCode() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    const int& getCapacity() const;
    const std::string& getTutorId() const;
    const std::string& getDay() const;
    const std::string& getStartTime() const;
    const std::string& getEndTime() const;

    void setId(const std::string& id);
    void setCode(const std::string& code);
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCapacity(const int& capacity);
    void setTutorId(const std::string& tutorId);
    void setDay(const std::string& day);
    void setStartTime(const std::string& startTime);
    void setEndTime(const std::string& endTime);

    //Stream operator declarations for file I/O
    friend std::ostream& operator<<(std::ostream& os, const Course& course);
    friend std::istream& operator>>(std::istream& is, Course& course);

    ~Course() {};

private:
    std::string id_;
    std::string code_;
    std::string title_;
    std::string description_;
    int capacity_;
    std::string tutorId_;
    std::string day_;
    std::string startTime_;
    std::string endTime_;
};

#endif // COURSE_H