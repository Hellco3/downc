#include <iostream>
#include <string>
#include <memory>

using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::string;

class File_Unit
{
public:
    File_Unit(double dsize)
        : _dsize(dsize)
    {
    }
    friend ostream &operator<<(ostream &output, const File_Unit &F);

private:
    double _dsize;
};