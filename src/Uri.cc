#include "Uri.h"

UriStruct::UriStruct() : port(0), hasPassword(false), ipv6LiteralAddress(false)
{
}

UriStruct::UriStruct(const UriStruct &c) = default;

UriStruct::~UriStruct() = default;

UriStruct &UriStruct::operator=(const UriStruct &c)
{
    if (this != &c)
    {
        protocol = c.protocol;
        host = c.host;
        dir = c.dir;
        file = c.file;
        query = c.query;
        username = c.username;
        password = c.password;
        port = c.port;
        hasPassword = c.hasPassword;
        ipv6LiteralAddress = c.ipv6LiteralAddress;
    }
    return *this;
}

void UriStruct::swap(UriStruct &other)
{
    using std::swap;
    if (this != &other)
    {
        swap(protocol, other.protocol);
        swap(host, other.host);
        swap(dir, other.dir);
        swap(file, other.file);
        swap(query, other.query);
        swap(username, other.username);
        swap(password, other.password);
        swap(port, other.port);
        swap(hasPassword, other.hasPassword);
        swap(ipv6LiteralAddress, other.ipv6LiteralAddress);
    }
}

static std::string printSubstring(const std::string& str, int offset, int length) {
    std::string retstr="";
    int strLength = str.length();
    int end = offset + length;

    if (offset < 0 || offset >= strLength || end > strLength) {
        return "";
    }

    for (int i = offset; i < end; i++) {
        retstr += str[i];
    }
    return retstr;
}
struct UriStruct getUriStruct(const std::string& uri)
{
    struct UriStruct uri_s;
    std::string tmpstr;
    int ret = 0;
    uri_split_result res;
    ret = uri_split(&res,uri.c_str());
    for(int i=0; i<10; i++)
    {
        if(res.field_set & (1 << i))
        {
            tmpstr = printSubstring(uri, res.fields[i].off, res.fields[i].len);
            switch(i) {
                case USR_SCHEME:
                {
                    uri_s.protocol = tmpstr;
                    break;
                }
                case USR_HOST:
                {
                    uri_s.host = tmpstr;
                    break;
                }
                case USR_PORT:
                {
                    uri_s.port = res.port;
                    break;
                }
                case USR_PATH:
                {   
                    uri_s.dir = tmpstr;
                    break;
                }
                case USR_QUERY:
                {
                    uri_s.query = tmpstr;
                    break;
                }
                case USR_USER:
                {
                    uri_s.username = tmpstr;
                    break;
                }
                case USR_PASSWD:
                {
                    uri_s.hasPassword = true;
                    uri_s.password = tmpstr; 
                    break;
                } 
                case USR_BASENAME:
                {
                    uri_s.file = tmpstr;
                    break;
                }
            }
        }
    }
    if(1 & res.flags)
        uri_s.ipv6LiteralAddress = true;
    return uri_s;
}