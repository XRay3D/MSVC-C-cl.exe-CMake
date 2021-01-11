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
    args.reserve(argc - 1);
    for (int i = 1; i < argc; ++i)
        args << QString::fromLocal8Bit(argv[i]);
    proc.setArguments(args);
    connect(&proc, &QProcess::readyReadStandardError, this, &Main::readStdErr);
    connect(&proc, &QProcess::readyReadStandardOutput, this, &Main::readStdOut);
    connect(&proc, qOverload<int>(&QProcess::finished), &QCoreApplication::exit);
    proc.start();
}

void Main::readStdErr()
{
    proc.setReadChannel(QProcess::StandardError);
    QByteArray ba(
        encoder->fromUnicode(
            decoder->toUnicode(
                proc.readAll())));
    fwrite(ba.constData(), 1, ba.size() - 1, stderr);
    fflush(stderr);
}

void Main::readStdOut()
{
    proc.setReadChannel(QProcess::StandardOutput);
    QByteArray ba(
        encoder->fromUnicode(
            decoder->toUnicode(
                proc.readAll())));
    fwrite(ba.constData(), 1, ba.size() - 1, stdout);
    fflush(stdout);
}
