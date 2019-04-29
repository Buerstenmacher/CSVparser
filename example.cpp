#include <iostream>
#include "CSVparser.hpp"

int main(int argc, char **argv)
{
        try
    {
        csv::Parser file = csv::Parser("files/readme.csv");

        std::cout << file[0][0] << std::endl; // display : 1997
        std::cout << file[0] << std::endl; // display : 1997 | Ford | E350

        std::cout << file[1]["Model"] << std::endl; // display : Cougar

        std::cout << file.rowCount() << std::endl; // display : 2
        std::cout << file.columnCount() << std::endl; // display : 3

        std::cout << file.getHeaderElement(2) << std::endl; // display : Model
    }
    catch (csv::Error &e)
    {
        std::cerr << e.what() << std::endl;
    }
        return 0;
}

