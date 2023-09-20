#include "./Profile.h"

#include "../Analytics/Analytics.h"
#include "../Menu/Menu.h"
#include "../Order/Order.h"

int Profile::getTerminalWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}
void Profile::centeredText(const std::string &text) {
    int terminalWidth = getTerminalWidth();

    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;
    // cout << "\033[1;31mbold red text\033[0m\n";
    std::cout << std::setw(padding + textWidth)
              << "\033[1;40m" + text + "\033[0m\n"
              << std::endl;
}
void Profile::showProfilePage() {
    Order order;
    Menu menu;
    Table profileTable, choiceTable;
    system("clear");
    centeredText("Hello!");
    centeredText("Welcome to Your Profile.");
    int choice;
    profileTable.add_row({"1. Menu", "2. Order", "3. Analytics", "4. Exit"});
    profileTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << profileTable << std::endl;
    choiceTable.add_row({"Enter Your Choice: "});
    choiceTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << choiceTable;
    std::cout << "\t";
    std::cin >> choice;
    std::cout << std::endl;
    switch (choice) {
        case 1:
            menu.firstPage();
            break;
        case 2:
            order.firstPage();
            break;
        case 3:
            Analytics analytics;
            analytics.firstPage();
            break;
        case 4:
            centeredText("Thank you!");
            break;
        default:
            centeredText("Select from the options given above");
    }
}
