#ifndef QMEDIAINFO_H
#define QMEDIAINFO_H

#include <memory>
#include <QStringList>

namespace MediaInfoDLL {
class MediaInfo;
}

class QMediaInfo {
public:
  static QMediaInfo& GetInst();
  bool Open(const QString& filename);
  int VidDurationLengthQuick(const QString& vidAbsPath);
  int AudDurationLengthQuick(const QString& audioAbsPath);
  QList<int> batchVidsDurationLength(const QStringList& vidsAbsPath);

private:
  int DurationLengthQuick(const QString& fileAbsPath, const int streamTypeInt);

  explicit QMediaInfo();
  QMediaInfo(const QMediaInfo& rhs) = delete;

  bool isLoadDllOk() { return m_bLoadDllResult; }
  std::unique_ptr<MediaInfoDLL::MediaInfo> m_mediaInfo;
  const bool m_bLoadDllResult;
};

#endif // QMEDIAINFO_H
