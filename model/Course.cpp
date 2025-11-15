#include "Course.h"

Course::Course(const std::string& id, const std::string& title, const std::string& description)
    : id_(id), title_(title), description_(description) {}

const std::string& Course::getId() const { return id_; }
const std::string& Course::getTitle() const { return title_; }
const std::string& Course::getDescription() const { return description_; }

void Course::setId(const std::string& id) { id_ = id; }
void Course::setTitle(const std::string& title) { title_ = title; }
void Course::setDescription(const std::string& description) { description_ = description; }
