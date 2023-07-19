#include "ProtocolDetector.h"

#include <cstring>
#include <iomanip>
#include "UriSplit.h"

ProtocolDetector::ProtocolDetector() = default;

ProtocolDetector::~ProtocolDetector() = default;

bool ProtocolDetector::isStreamProtocol(const std::string &uri)
{
  return uri_split(nullptr, uri.c_str()) == 0;
}

bool ProtocolDetector::guessTorrentFile(const std::string &uri)
{
  return false;
}

bool ProtocolDetector::guessTorrentMagnet(const std::string &uri)
{
  return false;
}

bool ProtocolDetector::guessMetalinkFile(const std::string &uri)
{
  return false;
}
