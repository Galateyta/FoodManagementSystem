#include "./Analytics.h"

#include <tabulate/table.hpp>

#include "../Menu/Menu.h"
#include "../Profile/Profile.h"
#include "../calculations/Calculations.h"
#include "../globalStates/GlobalStates.h"

int Analytics::getTerminalWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void Analytics::centeredText(const std::string &text) {
    int terminalWidth = getTerminalWidth();

    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;
    std::cout << std::setw(padding + textWidth)
              << "\033[1;40m" + text + "\033[0m\n"
              << std::endl;
}
void Analytics::showAnalytics() {
    std::string currentUserID = GlobalStates::currentUserID;
    float frequency = calculateFrequencyOnOrder(false, currentUserID);
    float coefficient = calculateCoefficientOnOrder(false, currentUserID);
    Table columns, data;
    centeredText("--------- You can see your Food Service Analytics ---------");
    columns.add_row({"1. Frequency", "2. Coefficient"});
    columns[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << columns << std::endl;
    data.add_row({std::to_string(frequency), std::to_string(coefficient)});
    data[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << data << std::endl;
    firstPage();
}

void Analytics::firstPage()
{
    Profile profile;
    Table firstPage;
    centeredText("--------- Welcome to Analytics Page ---------");
    firstPage.add_row({"1. Show Analytics", "2. Profile", "3. Exit"});
    firstPage[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << firstPage << std::endl;
    int choice;
    centeredText("Enter your choice: ");
    std::cin >> choice;
    switch (choice) {
        case 1:
            showAnalytics();
            break;
        case 2:
            profile.showProfilePage();
            break;
        case 3:
            centeredText("Thank you!");
            break;
        default:
            centeredText("Select from the options given above");
    }
}