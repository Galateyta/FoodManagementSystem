#include "./Profile.h"

#include "../Analytics/Analytics.h"
#include "../Menu/Menu.h"
#include "../Order/Order.h"

void Profile::showProfilePage() {
    Order order;
    Menu menu;

    std::cout << "\t\t\t\t --------- Hello!--------- \n\n";
    std::cout << "\t\t\t\t  Welcome to Your Profile. \n\n\n";
    int choice;
    std::cout << "\t * Press 1 for Menu Screen " << std::endl;
    std::cout << "\t * Press 2 for Ordering " << std::endl;
    std::cout << "\t * Press 3 for Analytics" << std::endl;
    std::cout << "\t * Press 4 to EXIT \n\n";
    std::cout << "\t Enter your choice: ";
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
            std::cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            std::cout << "\x1B[2J\x1B[H";
            std::cout << "\t\t\t Select from the options given above \n";
    }
}
