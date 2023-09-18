#include "./Analytics.h"

#include "../Profile/Profile.h"
#include "../calculations/Calculations.h"
#include "../globalStates/GlobalStates.h"

int Analytics::getTerminalWidth()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void Analytics::centeredText(const std::string &text)
{
    int terminalWidth = getTerminalWidth();

    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;

    std::cout << std::setw(padding + textWidth) << text << std::endl;
}
void Analytics::header()
{
    system("clear");
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("\033[32m FOOD MANAGEMENT ");
};

void Analytics::showAnalytics()
{
    std::string currentUserID = GlobalStates::currentUserID;

    float frequency = calculateFrequencyOnOrder(false, currentUserID);
    float coefficient = calculateCoefficientOnOrder(false, currentUserID);

    std::cout << "Frequency - " << frequency << std::endl;
    std::cout << "Coefficient - " << coefficient << std::endl;
    firstPage();
}

void Analytics::firstPage()
{
    Profile profile;

    header();
    centeredText("= --------- Welcome to Analytics Page --------- ");
    int choice;
    centeredText(" * Press 1 to Show Analytics ");
    centeredText(" * Press 2 to Go Back to Profile ");
    centeredText(" * Press 3 to EXIT ");
    std::cout << "\t Enter your choice: ";
    std::cin >> choice;
    switch (choice)
    {
    case 1:
        showAnalytics();
        break;
    case 2:
        profile.showProfilePage();
        break;
    case 3:
        header();
        centeredText(" Thank you! ");
        break;
    default:

        header();
        centeredText(" Select from the options given above ");
    }
}