#include <iostream>
#include "CSVparser.hpp"

int main(int argc, char **argv)	{
try    {
        csv::Parser file = csv::Parser("files/insurance.csv");
        std::cout << file.at(0) << std::endl; 			// display line 0
        std::cout << file.at(1) << std::endl; 			// display line 1
        std::cout << file.at(2) << std::endl; 			// display line 2

        std::cout << file.getHeaderElement(1) << std::endl;	//it's "statecode"

        std::cout << "Output as std::string: " << std::endl;
        std::cout << file.at(0).at(0) << std::endl;
        std::cout << file.at(1).at(file.getHeaderElement(0)) << std::endl;

        std::cout << "Output as double: " << std::endl;
        std::cout << file.at(0).getValue(file.getHeaderElement(0))*0.5*2.0  << std::endl;
        std::cout << file.at(1).getValue(0)*0.5*2.0  << std::endl;

        std::cout << file.at(file.rowCount()-1) << std::endl; 			// display last line

        std::cout << "Filesize is: " << file.rowCount() << " x " << file.columnCount() << std::endl;
	auto lati = file.getColumnValues("point_latitude");
	auto longi = file.getColumnValues("point_longitude");
	double sum_la{0};
	double sum_lo{0};
	for (auto& la:lati) {sum_la +=la;}
	for (auto& lo:longi) {sum_lo +=lo;}
	sum_la /= lati.size();
	sum_lo /= lati.size();
        std::cout << "Average Value of \"point_latitude\" is:  " << sum_la<< std::endl;
        std::cout << "Average Value of \"point_longitude\" is: " << sum_lo<< std::endl;



	file.sync();
	}
catch (csv::Error &e)	{std::cerr << e.what() << std::endl;}
return 0;
}

