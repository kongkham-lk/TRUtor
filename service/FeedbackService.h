#pragma once
#include <string>
#include <vector>

struct Feedback {
    int tutorId;
    int studentId;
    std::string content;
};

class FeedbackService {
public:
    FeedbackService(const std::string& filePath = "feedback.txt");

    void addFeedback(int studentId, int tutorId, const std::string& content);
    std::vector<Feedback> getFeedbackForTutor(int tutorId) const;

private:
    std::string filePath;
};