CSVparser
=========

CSVparser intends to be a simple C++ parser for the CSV file format.

What is a CSV file ?
--------------------

CSV is a common, relatively simple file format that is widely supported by consumer, business, and scientific applications. Among its most common uses is moving tabular data between programs that natively operate on incompatible (often proprietary and/or undocumented) formats. This works because so many programs support some variation of CSV at least as an alternative import/export format.

Compile and run
---------------

```bash
g++		example.cpp	-o example	-std=c++11			-Wno-psabi			-Weffc++	-Wall	-pedantic	&&	./example
```

```
start compiler;	input file; 	output file; 	use at least c++ 2011 standard; dont tell us stupid warnings; 	show all other warnings and errors; 	and 	run it ;-)
```

Usage
-----

The following example shows how to make a simple usage of CSVparser.<br />
with file /files/insurance.csv:
-------------------------------
example.cpp:

```c++
#include <iostream>
#include "CSVparser.hpp"

int main(int argc, char **argv) {
try    {
        csv::Parser file = csv::Parser("files/insurance.csv");
        std::cout << file.at(0) << std::endl;                   // display line 0
        std::cout << file.at(1) << std::endl;                   // display line 1
        std::cout << file.at(2) << std::endl;                   // display line 2

        std::cout << file.getHeaderElement(1) << std::endl;     //it's "statecode"

        std::cout << "Output as std::string: " << std::endl;
        std::cout << file.at(0).at(0) << std::endl;
        std::cout << file.at(1).at(file.getHeaderElement(0)) << std::endl;

        std::cout << "Output as double: " << std::endl;
        std::cout << file.at(0).getValue(file.getHeaderElement(0))*0.5*2.0  << std::endl;
        std::cout << file.at(1).getValue(0)*0.5*2.0  << std::endl;

        std::cout << file.at(file.rowCount()-1) << std::endl;                   // display last line

        std::cout << "Filesize is: " << file.rowCount() << " x " << file.columnCount() << std::endl;
        file.sync();
        }
catch (csv::Error &e)   {std::cerr << e.what() << std::endl;}
return 0;
}
```


```
Output on my RaspberryPi with Raspbian is:

119736 | FL | CLAY COUNTY | 498960 | 498960 | 498960 | 498960 | 498960 | 792148.9 | 0 | 9979.2 | 0 | 0 | 30.102261 | -81.711777 | Residential | Masonry | 1 | 
448094 | FL | CLAY COUNTY | 1322376.3 | 1322376.3 | 1322376.3 | 1322376.3 | 1322376.3 | 1438163.57 | 0 | 0 | 0 | 0 | 30.063936 | -81.707664 | Residential | Masonry | 3 | 
206893 | FL | CLAY COUNTY | 190724.4 | 190724.4 | 190724.4 | 190724.4 | 190724.4 | 192476.78 | 0 | 0 | 0 | 0 | 30.089579 | -81.700455 | Residential | Wood | 1 | 
statecode
Output as std::string: 
119736
448094
Output as double: 
119736
448094
398149 | FL | PINELLAS COUNTY | 373488.3 | 373488.3 | 0 | 0 | 373488.3 | 596003.67 | 0 | 0 | 0 | 0 | 28.06444 | -82.77459 | Residential | Masonry | 1 | 
Filesize is: 36634 x 18
```
 
