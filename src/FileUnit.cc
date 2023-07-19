#include "FileUnit.h"

template <typename... Args>
string string_format(const string &format, Args... args)
{
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args...);
    char bytes[size];
    snprintf(bytes, size, format.c_str(), args...);
    return string(bytes);
}

ostream &operator<<(ostream &output, const File_Unit &F)
{
    string out = "";
    if (F._dsize > 1024.0f)
    {
        double dSizeKB = F._dsize / 1024.0f;
        if (dSizeKB > 1024.0f)
        {
            double dSizeMB = dSizeKB / 1024.0f;
            if (dSizeMB > 1024.0f)
            {
                double dSizeGB = dSizeMB / 1024.0f;
                out = string_format("%.2lfG", dSizeGB);
            }
            else
            {
                out = string_format("%.2lfM", dSizeMB);
            }
        }
        else
        {
            out = string_format("%.2lfKB", dSizeKB);
        }
    } else {
        if(F._dsize > 0)
        {
            out = string_format("%.2lfB", F._dsize);
        }
    }
    output << out;
    return output;
}