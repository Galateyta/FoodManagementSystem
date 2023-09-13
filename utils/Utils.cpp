#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>

std::string generateUniqueID() {
    // Unique ID generation algorithm. Source -
    // https://saturncloud.io/blog/algorithm-for-generating-a-unique-id-in-c/
    auto currentTime =
        std::chrono::system_clock::now().time_since_epoch().count();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    auto randomNumber = dis(gen);

    std::string combinedString =
        std::to_string(currentTime) + std::to_string(randomNumber);

    std::hash<std::string> hasher;
    std::size_t hashedValue = hasher(combinedString);

    std::string uniqueID = std::to_string(hashedValue);

    return uniqueID;
}

std::string timePointToString(
    const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
                  std::localtime(&time));
    return buffer;
}

void updateCurrentOilCount(std::string oilCount) {
    // TODO implement this
}