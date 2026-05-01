#include "QMediaInfo.h"
#include <QFileInfo>
#include <QDir>
int main(int argc, char* argv[]) {
  const QDir dir = QFileInfo(__FILE__).absoluteDir();
  QString vidFile{dir.absoluteFilePath(R"(OneNoteFRE_Welcome.mp4)")};
  QString audioFile{dir.absoluteFilePath(R"(download-complete.mp3)")};

  auto& inst = QMediaInfo::GetInst();
  int vidDur = inst.DurationLengthQuick(vidFile);
  int audDur = inst.DurationLengthQuick(audioFile);
  qDebug("file[%s] duration: %d", qPrintable(vidFile), vidDur);
  qDebug("file[%s] duration: %d", qPrintable(audioFile), audDur);
  return 0;
}
