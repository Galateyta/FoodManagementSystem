#ifndef GLOBALSTATES_H
#define GLOBALSTATES_H

#include <iostream>

class GlobalStates {
   public:
    GlobalStates(const std::string &userID);
    static std::string currentUserID;
    void setCurrentUserID(std::string userID);
};
#endif