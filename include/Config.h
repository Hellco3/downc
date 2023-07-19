#ifndef __CONFIG_HH__
#define __CONFIG_HH__
#include "mxml.h"
#include <string>

class Config
{
public:
    Config(const char *configPath, const char *rootKey);

    int getConfigString(std::string &s_data, const char *propertyTree);
    int _getConfigString(std::string &s_data, const char *propertyTree);
    long long getConfigLong(const char *propertyTree);
    int getConfigInt(const char *propertyTree);
    bool getConfigBool(const char *propertyTree);

    void setConfigString(const char *propertyTree, const char *text);
    void setConfigLong(const char *propertyTree, long long longValue);
    void setConfigInt(const char *propertyTree, int intValue);
    void setConfigBool(const char *propertyTree, bool boolValue);

    virtual ~Config();

private:
    void init(const char *configPath);
    mxml_node_t *advanceToElement(mxml_node_t *tempNode, const char *propertyTree);

    bool inited_;
    bool _commonInited;

    std::string _configPath;

    mxml_node_t *doc_;

    std::string _rootKey;

    /// Root node of Config
    mxml_node_t *configElement_;
};
#endif
