#include "FeedbackService.h"
#include <fstream>
#include <iostream>

FeedbackService::FeedbackService(const std::string& filePath)
    : filePath(filePath) {
}

void FeedbackService::addFeedback(int studentId, int tutorId, const std::string& content) {
    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open feedback file.\n";
        return;
    }

    file << tutorId << "," << studentId << "," << content << "\n";
}

std::vector<Feedback> FeedbackService::getFeedbackForTutor(int tutorId) const {
    std::vector<Feedback> results;
    std::ifstream file(filePath);
    if (!file.is_open()) return results;

    std::string line;
    while (getline(file, line)) {
        size_t firstComma = line.find(',');
        size_t secondComma = line.find(',', firstComma + 1);
        if (firstComma == std::string::npos || secondComma == std::string::npos) continue;

        int tId = std::stoi(line.substr(0, firstComma));
        int sId = std::stoi(line.substr(firstComma + 1, secondComma - firstComma - 1));
        std::string content = line.substr(secondComma + 1);

        if (tId == tutorId) {
            results.push_back({ tId, sId, content });
        }
    }

    return results;
}