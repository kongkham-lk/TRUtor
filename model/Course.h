#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
public:
    Course() {}
    Course(const std::string& id, const std::string& title, const std::string& description);

    const std::string& getId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;

    void setId(const std::string& id);
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);

private:
    std::string id_;
    std::string title_;
    std::string description_;
};

#endif // COURSE_H
