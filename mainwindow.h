#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <iostream>
#include <string>
#include <fstream>
#include <complex>
#include <valarray>
#include <vector>
#include <cmath>
#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QMediaPlayer>
#include "inc/plot.h"
#include "inc/rsa.h"

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

// http://soundfile.sapp.org/doc/WaveFormat/

struct WavHeader {
    char chunkID[4];
    int ChunkSize;
    char WAVE[4];
    char Subchunk1ID[4];
    int Subchunk1Size;
    short AudioFormat;
    short NumChannels;
    int SampleRate;
    int ByteRate;
    short BlockAlign;
    short BitsPerSample;
    char Subchunk2ID[4];
    int Subchunk2Size;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_openFileButton_clicked();

    void on_plotButton_clicked();

    void on_playOrgButton_clicked();

    void on_rsaButton_clicked();

    void on_saveAndPlayButton_clicked();

private:
    Ui::MainWindow *ui;
    WavHeader header;
    FILE *file;
    std::vector<short> buffor;
    CArray data; // tablica przechowujaca liczby zespolone
    std::vector <double> amplitude; // do aplitudy po fft
    std::vector<double> freq;  // wektor czestotliwosci
    QMediaPlayer* player;
    QString filepath;
    RSA rsa = RSA(1237,17);
};

#endif // MAINWINDOW_H
