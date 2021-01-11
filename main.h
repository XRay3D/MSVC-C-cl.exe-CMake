#pragma once

#include <QObject>
#include <QProcess>
#include <QTextCodec>

class Main : public QObject {
    //    Q_OBJECT
public:
    void start(int argc, char** argv);

private:
    QProcess proc;
    QTextCodec *decoder, *encoder;

    void readStdErr();
    void readStdOut();
};
