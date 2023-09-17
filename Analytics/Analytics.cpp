#include "./Analytics.h"

#include "../Profile/Profile.h"
#include "../calculations/Calculations.h"

void Analytics::showAnalytics() {
    std::string restID = "Tashir_ID";
    std::string currID = "2196617506373257558";

    float frequency = calculateFrequencyOnOrder(false, restID, currID);
    float coefficient = calculateCoefficientOnOrder(false, currID);

    std::cout << "Frequency - " << frequency << std::endl;
    std::cout << "Coefficient - " << coefficient << std::endl;
    firstPage();
}

void Analytics::firstPage() {
    Profile profile;

    std::cout << "\t\t\t --------- Welcome to Analytics Page --------- \n\n";
    int choice;
    std::cout << "\t * Press 1 to Show Analytics " << std::endl;
    std::cout << "\t * Press 2 to Go Back to Profile " << std::endl;
    std::cout << "\t * Press 3 to EXIT \n\n";
    std::cout << "\t Enter your choice: ";
    std::cin >> choice;
    switch (choice) {
        case 1:
            showAnalytics();
            break;
        case 2:
            profile.showProfilePage();
            break;
        case 3:
            std::cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\t\t\t Select from the options given above \n";
    }
}