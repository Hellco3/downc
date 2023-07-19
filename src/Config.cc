#include "Config.h"
#include <iostream>
#include <string.h>

const char *whitespace_cb(mxml_node_t *node, int where);

Config::Config(const char *configPath, const char *rootKey)
: _configPath(configPath)
{
    // cout << "OsdConfig create new" << endl;
    if (nullptr != rootKey)
    {
        _rootKey = rootKey;
    }
    else
    {
        std::cerr << "rootKey == null" << std::endl;
        return;
    }
    init(configPath);
}

int Config::getConfigString(std::string &s_data, const char *propertyTree)
{
    return _getConfigString(s_data, propertyTree);
}

// 由上层传入string获取结果，调用advanceToElement传入TiXmlHandle实例，以解决原调用过程涉及的返回值指向临时变量存储空间的问题
int Config::_getConfigString(std::string &s_data, const char *propertyTree)
{
    mxml_node_t *tempNode = advanceToElement(configElement_, propertyTree);
    if (!tempNode)
    {
        return -1;
    }
    s_data = mxmlGetOpaque(tempNode);
    return 0;
}

long long Config::getConfigLong(const char *propertyTree)
{
    std::string s_data;
    int ret = _getConfigString(s_data, propertyTree);
    if (0 != ret)
    {
        return static_cast<long long>(ret);
    }
    return atol(s_data.c_str());
}

int Config::getConfigInt(const char *propertyTree)
{
    std::string s_data;
    int ret = _getConfigString(s_data, propertyTree);
    if (0 != ret)
    {
        return ret;
    }
    return atoi(s_data.c_str());
}

bool Config::getConfigBool(const char *propertyTree)
{
    std::string s_data;
    int ret = _getConfigString(s_data, propertyTree);
    if (0 != ret)
    {
        return false;
    }
    if("true" == s_data || "1" == s_data)
        return true;
    else 
        return false;
}

void Config::setConfigString(const char *propertyTree, const char *text)
{
    mxml_node_t *tempNode = configElement_;
    tempNode = advanceToElement(tempNode, propertyTree);
    mxml_node_t *oldNode = mxmlGetFirstChild(tempNode);
    mxmlSetText(oldNode, 0, text);
}

void Config::setConfigLong(const char *propertyTree, long long longValue)
{
    char tempString[1024] = {0};
    sprintf(tempString, "%lld", longValue);
    setConfigString(propertyTree, tempString);
}

void Config::setConfigInt(const char *propertyTree, int intValue)
{
    char tempString[1024] = {0};
    sprintf(tempString, "%d", intValue);
    setConfigString(propertyTree, tempString);
}

void Config::setConfigBool(const char *propertyTree, bool boolValue)
{
    if (boolValue)
        setConfigString(propertyTree, "true");
    else
        setConfigString(propertyTree, "false");
}

void Config::init(const char *configPath)
{
    FILE *fp;

    fp = fopen(configPath, "r+");
    if (fp == NULL)
    {
        std::cerr << "Failed to open " << configPath << std::endl;
        return;
    }
    doc_ = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
    fclose(fp);
    if (!doc_)
    {
        std::cerr << "Failed to load " << configPath << std::endl;
    }

    configElement_ = mxmlFindElement(doc_, doc_, _rootKey.c_str(), NULL, NULL, MXML_DESCEND);
}

/// TODO: Read from Common Config if "Common" Tag is encountered
mxml_node_t *Config::advanceToElement(mxml_node_t *tempNode, const char *propertyTree)
{
    int len = strlen(propertyTree);
    char propertyTree_[len + 1];
    char *namePtr = nullptr;
    char *tmp_ptr = nullptr;

    memset(propertyTree_, 0, sizeof(propertyTree_));
    strncpy(propertyTree_, propertyTree, len);

    namePtr = strtok_r(propertyTree_, ">", &tmp_ptr);
    while (namePtr)
    {
        tempNode = mxmlFindElement(tempNode, doc_, namePtr, NULL, NULL, MXML_DESCEND);
        if (!tempNode)
        {
            // fprintf(stderr,"%s: %s Not in Specific Config, Cont. to Common Config\n",propertyTree,namePtr);
            break;
        }
        namePtr = strtok_r(nullptr, ">", &tmp_ptr);
    }

    return tempNode;
}

Config::~Config()
{
    FILE *fp = fopen(_configPath.c_str(), "r+");
    mxmlSaveFile(doc_, fp, MXML_NO_CALLBACK);
    fclose(fp);
    mxmlDelete(doc_);
}

const char *whitespace_cb(mxml_node_t *node, int where)
{
    return NULL;
}