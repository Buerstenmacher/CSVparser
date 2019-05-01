#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace csv {

class Error : public std::runtime_error {
public:
Error(const std::string &msg):std::runtime_error(std::string("CSVparser : ").append(msg)) {}
};//class Error //////////////////////////////////////////////////////////////////////////////////

class Row {
private:
const std::vector<std::string> _header;
std::vector<std::string> _values;

public:
Row(const std::vector<std::string> &header): _header(header),_values{} {}
~Row(void) {}

size_t size(void) const  {return _values.size();}
void push(const std::string &value) {_values.push_back(value);}

bool set(const std::string &key, const std::string &value) {
for (auto it = _header.begin(); it != _header.end(); it++) {
	if (key == *it) {
		_values.at(std::distance(_header.begin(),it)) = value;
		return true;
		}
	}
return false;
}

std::string operator[](size_t valuePosition) const  {
if (valuePosition < _values.size())	{return _values.at(valuePosition);}
throw Error("can't return this value because it doesn't exist");
}

std::string operator[](const std::string &key) const {
for (auto it = _header.begin(); it != _header.end(); it++) {if (key == *it)	{return _values.at(std::distance(_header.begin(),it));}}
throw Error("can't return this value because it doesn't exist");
}

template<class keytype=size_t>
std::string at(keytype vp) const	{return (*this)[vp];}

template<typename T=double,class keytype>
T getValue(keytype pos) const {
T res{};
std::stringstream ss{};
ss << (*this).at(pos);
ss >> res;
return res;
}

friend std::ostream& operator<<(std::ostream& os, const Row &row);
friend std::ofstream& operator<<(std::ofstream& os, const Row &row);
};//class Row  //////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &os, const Row &row) {
for (auto& ref:row._values)	{os << ref << " | ";}
return os;
}

std::ofstream &operator<<(std::ofstream &os, const Row &row) {	//ToDo check if it works correct on '\r' and '\n' 
for (size_t i{0}; i != row._values.size(); i++) {
	os << row._values.at(i);
	if (i < row._values.size() - 1)	{os << ",";}
	}
return os;
}///////////////////////////////////////////////////////////////////////////////////////////

enum DataType {eFILE = 0,ePURE = 1};

class Parser {
private:
std::string _file;	//path and name
const DataType _type;
const char _sep;	//should be ','
std::vector<std::string> _originalFile{}, _header{};
std::vector<Row*> _content;

void parseHeader(void) {
std::stringstream ss(_originalFile.at(0));
std::string item;
while (std::getline(ss, item, _sep)) {_header.push_back(item);}
}

void parseContent(void)	{
for (auto it = ++this->_originalFile.begin(); it !=_originalFile.end(); it++)	{
	bool quoted = false;
	size_t tokenStart{0};
	Row *row = new Row(_header);
	for (size_t i{0}; i != it->length(); i++)	{
		if (it->at(i) == '"')	{quoted = ((quoted) ? (false) : (true));}
		else if (it->at(i) == ',' && !quoted) {
			row->push(it->substr(tokenStart, i - tokenStart));
			tokenStart = i + 1;
			}
		}//end
	row->push(it->substr(tokenStart, it->length() - tokenStart));
	if (row->size() != _header.size()) {throw Error("corrupted data !");}	//if value(s) missing
	_content.push_back(row);
	}
}

public:
Parser(const std::string& data, const DataType& type = eFILE, char sep = ',')
	:_file{},_type(type),_sep(sep),_originalFile{},_header{},_content{} {
std::string line{};
if (type == eFILE) {
	_file = data;
	std::ifstream ifile(_file.c_str());
	if (ifile.is_open()) {
		while (ifile.good()) {
                	getline(ifile, line);
			if (line != "")	{_originalFile.push_back(line);}
			}
		ifile.close();
		if (_originalFile.size() == 0)	{throw Error(std::string("No Data in ").append(_file));}
		parseHeader();
		parseContent();
        	}
	else	{throw Error(std::string("Failed to open ").append(_file));}
	}
else {	std::istringstream stream(data);
	while (std::getline(stream, line))	{if (line != "") {_originalFile.push_back(line);}}
	if (_originalFile.size() == 0)		{throw Error(std::string("No Data in pure content"));}
	parseHeader();
	parseContent();
	}
}

~Parser(void) {for(auto ptr:_content)	{delete ptr;}}

const Row& getRow(size_t rowPosition) const {
if (rowPosition < _content.size())	{return *(_content.at(rowPosition));}
throw Error("can't return this row (doesn't exist)");
}

size_t rowCount(void) const 			{return _content.size();}
size_t columnCount(void) const 			{return _header.size();}
std::vector<std::string> getHeader(void) const 	{return _header;}
std::string getFileName(void) const 		{return _file;}
const Row& operator[](size_t rowPosition) const 	{return Parser::getRow(rowPosition);}
const Row& at(size_t rp) const			{return (*this)[rp];}

const std::string getHeaderElement(size_t pos) const {
if (pos >= _header.size()) {throw Error("can't return this header (doesn't exist)");}
return _header.at(pos);
}

bool deleteRow(size_t pos) {
if (pos < _content.size()) {
	delete *(_content.begin() + pos);
	_content.erase(_content.begin() + pos);
	return true;
	}
return false;
}

bool addRow(size_t pos, const std::vector<std::string> &r) {
Row *row = new Row(_header);
for (auto& ref:r) {row->push(ref);}
if (pos <= _content.size()) {
	_content.insert(_content.begin() + pos, row);
	return true;
	}
return false;
}

void sync(void) const  {
if (_type == DataType::eFILE) {
	std::ofstream f(_file, std::ios::out | std::ios::trunc);
	for (size_t i{0};i != _header.size(); ++i) {	// header
		f << _header.at(i);
		if (i < _header.size() - 1)	{f << ",";}
		else				{f << std::endl;}
		}
	for (auto it=_content.begin(); it!=_content.end();it++)	{f << **it << std::endl;}
	f.close();
	}
}

};	//class Parser
}	//namespace csv
#endif 	//CSVPARSER_HPP
