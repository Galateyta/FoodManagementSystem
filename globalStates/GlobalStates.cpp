#include "./GlobalStates.h"

std::string GlobalStates::currentUserID;

GlobalStates::GlobalStates(const std::string& userID) {
    currentUserID = userID;
}
void GlobalStates::setCurrentUserID(std::string userID) {
    std::cout << userID << std::endl;
    currentUserID = userID;
}