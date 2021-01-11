#include "main.h"

#include <QCoreApplication>
#include <QProcess>
#include <stdio.h>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    Main m;
    m.start(argc, argv);
    return a.exec();
}

void Main::start(int argc, char** argv)
{
    decoder = QTextCodec::codecForName("IBM866");
    encoder = QTextCodec::codecForName("UTF-8");
    proc.setProgram("cl_real.exe");
    QStringList args;
    for (int i = 1; i < argc; ++i) {
        args << QString::fromLocal8Bit(argv[i]);
    }
    proc.setArguments(args);
    connect(&proc, &QProcess::readyReadStandardError,
        this, &Main::readStdErr);
    connect(&proc, &QProcess::readyReadStandardOutput,
        this, &Main::readStdOut);
    connect(&proc, qOverload<int /*, QProcess::ExitStatus*/>(&QProcess::finished),
        this, &Main::onExit);
    proc.start();
}

void Main::readStdErr()
{
    proc.setReadChannel(QProcess::StandardError);
    QByteArray ba = proc.readAll();
    QString s = decoder->toUnicode(ba);
    ba = encoder->fromUnicode(s);
    fwrite(ba.constData(), 1, ba.size(), stderr);
    fflush(stderr);
}

void Main::readStdOut()
{
    proc.setReadChannel(QProcess::StandardOutput);
    QByteArray ba = proc.readAll();
    QString s = decoder->toUnicode(ba);
    ba = encoder->fromUnicode(s);
    fwrite(ba.constData(), 1, ba.size(), stdout);
    fflush(stdout);
}

void Main::onExit(int code)
{
    QCoreApplication::exit(code);
}
