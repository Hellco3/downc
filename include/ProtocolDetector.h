#ifndef D_PROTOCOL_DETECTOR_H
#define D_PROTOCOL_DETECTOR_H

#include <string>

// 协议检测类，用于后期扩展BT下载和磁链下载
class ProtocolDetector
{
public:
  ProtocolDetector();

  ~ProtocolDetector();

  // Returns true if uri is http(s)/ftp, otherwise returns false.
  static bool isStreamProtocol(const std::string &uri);

  // Returns true if ProtocolDetector thinks uri is a path of BitTorrent
  // metainfo file, otherwise returns false.
  static bool guessTorrentFile(const std::string &uri);

  // Returns true if ProtocolDetector thinks uri is BitTorrent Magnet link.
  // magnet:?xt=urn:btih:<info-hash>...
  static bool guessTorrentMagnet(const std::string &uri);

  // Returns true if ProtocolDetector thinks uri is a path of Metalink XML
  // file, otherwise return false.
  static bool guessMetalinkFile(const std::string &uri);
};

#endif // D_PROTOCOL_DETECTOR_H
