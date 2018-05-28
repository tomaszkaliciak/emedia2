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
#include "plot.h"

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


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
    void f();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

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
};

#endif // MAINWINDOW_H
