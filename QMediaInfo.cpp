#include "QMediaInfo.h"
#include "MediaInfoDLL.h"
#include <QTime>

inline MediaInfoDLL::String ToMediaInfoString(const QString& qstr) {
#if defined(UNICODE) || defined(_UNICODE)
  return (qstr).toStdWString().c_str();
#else
  return (qstr).toStdString().c_str();
#endif
}

inline QString fromMediaInfoString(const MediaInfoDLL::String& mStr) {
#if defined(UNICODE) || defined(_UNICODE)
  return QString::fromStdWString(mStr);
#else
  return QString::fromStdString(mStr);
#endif
}

#include <QLibrary>
QMediaInfo::QMediaInfo()
  : m_mediaInfo{new MediaInfoDLL::MediaInfo}
  , m_bLoadDllResult{QLibrary{"MediaInfo.dll"}.load()} {
  qWarning("load dll[MediaInfo.dll] result: %d", m_bLoadDllResult);
}

QMediaInfo& QMediaInfo::GetInst() {
  static QMediaInfo inst;
  return inst;
}

bool QMediaInfo::Open(const QString& filename) {
  if (!isLoadDllOk()) {
    return false;
  }
  return m_mediaInfo->Open(ToMediaInfoString(filename)) != 0;
}

int QMediaInfo::VidDurationLengthQuick(const QString& vidAbsPath) {
  return DurationLengthQuick(vidAbsPath, MediaInfoDLL::stream_t::Stream_Video);
}

int QMediaInfo::AudDurationLengthQuick(const QString& audioAbsPath) {
  return DurationLengthQuick(audioAbsPath, MediaInfoDLL::stream_t::Stream_Audio);
}

int QMediaInfo::DurationLengthQuick(const QString& fileAbsPath, const int streamTypeInt) {
  if (!Open(fileAbsPath)) {
    return -1;
  }
  MediaInfoDLL::stream_t streamType = static_cast<MediaInfoDLL::stream_t>(streamType);

  using namespace MediaInfoDLL;
  String mediaInfoDur = m_mediaInfo->Get(streamType,
                                         0,                       //
                                         __T("Duration/String3"), //
                                         info_t::Info_Text,
                                         info_t::Info_Name);
  const QString& qStringDur = fromMediaInfoString(mediaInfoDur);
  return QTime::fromString(qStringDur, Qt::ISODateWithMs).msecsSinceStartOfDay();
}

QList<int> QMediaInfo::batchVidsDurationLength(const QStringList& vidsAbsPath) {
  if (vidsAbsPath.isEmpty()) {
    qDebug("Input vids abs path list is empty");
    return {};
  }

  QList<int> durationList;
  durationList.reserve(vidsAbsPath.size());
  for (const QString& filename : vidsAbsPath) {
    durationList.push_back(VidDurationLengthQuick(filename));
  }
  return durationList;
}
