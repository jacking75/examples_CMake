#include <iostream>
#include <Poco/RegularExpression.h>

int main() {
    Poco::RegularExpression regexp("^[a-zA-Z]+");

    std::string buf;
    regexp.extract("ABC123", buf);
    std::cout << buf << std::endl; //=> ABC

    return 0;
}