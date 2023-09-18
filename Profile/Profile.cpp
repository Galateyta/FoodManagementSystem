#include "./Profile.h"

#include "../Analytics/Analytics.h"
#include "../Menu/Menu.h"
#include "../Order/Order.h"
void Profile::showProfilePage()
{
    Order order;
    Menu menu;
    order.header();
    order.centeredText(" --------- Hello!--------- ");
    order.centeredText(" Welcome to Your Profile. ");
    int choice;
    order.centeredText(" * Press 1 for Menu Screen ");
    order.centeredText(" * Press 2 for Ordering ");
    order.centeredText(" * Press 3 for Analytics");
    order.centeredText(" * Press 4 to EXIT ");
    std::cout << " Enter your choice: ";
    std::cin >> choice;
    std::cout << std::endl;

    switch (choice)
    {
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
        order.header();
        order.centeredText(" Thank you! ");
        break;
    default:
        order.header();
        order.centeredText("Select from the options given above ");
    }
}
