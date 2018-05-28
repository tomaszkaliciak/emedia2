#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;
QT_CHARTS_USE_NAMESPACE


const double PI = 3.141592653589793238460;


void fft(CArray& x) {
    const size_t N = x.size();
    if (N <= 1) return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k) {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

// http://soundfile.sapp.org/doc/WaveFormat/





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                   tr("Wybierz plik .wav",
                                   "~/"),
                                   "Music file (*.wav)");
    file = fopen(filename.toUtf8(), "rb");
    fread(&header, sizeof(header), 1, file);
    long numberOfSamples = (8 * header.Subchunk2Size) / (header.NumChannels * header.BitsPerSample);
    buffor.resize(numberOfSamples);
    data.resize(numberOfSamples);
    fread(&buffor[0], sizeof(vector<short>::value_type), buffor.size(),file);
    fclose(file);
    filepath = filename;
}



void MainWindow::on_pushButton_2_clicked()
{
    amplitude.clear();
    freq.clear();

    for(uint i = 0; i < buffor.size(); ++i) {
        int sample = buffor[i];
        double nSample = sample; // normalizacja danych do zakresu (-1,1)
        data[i] = Complex(nSample);      // dodaj do tablicy liczb zespolonych
    }

 //   fft(data);

   // data = abs(data); // modul l. zespolonej

    QLineSeries *series = new QLineSeries();
    for(uint i = 0; i < data.size()/2; ++i) {
        amplitude.push_back(real(data[i])); // kopiuj do wektora czesc rzeczywista
        freq.push_back((i * header.SampleRate) / (double)data.size());
        series->append(freq[i],amplitude[i]);
    }
    QChart *chart = new QChart();
       chart->legend()->hide();
       chart->addSeries(series);
       chart->createDefaultAxes();
       chart->setTitle("FFT");
       Plot* plot = new Plot(chart);
       plot->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(filepath));
    player->setVolume(50);
    player->play();
}


void MainWindow::on_pushButton_3_clicked()
{
    QString cb = ui->comboBox->currentText();
   // std::cout << cb.toStdString() << std::endl;
    unsigned long long p = 1237;
    unsigned long long q = 17;
    unsigned long long n = p * q;
    unsigned long long fi = (p-1) * (q-1);

    // zeby klucz publiczny nie był zbyt mały zacznij szukac od polowy n
    uint e = n/2;

    while (gcd(fi,e) > 1 && e < n) {
        ++e;
    }

    short  r;
    for(uint i = 0; i < data.size(); ++i) {
        if(buffor[i] < 0) {
             r = -1 * buffor[i];
             r = powerModulo(r,e,n);
             r = -1 * r;
             buffor[i] = r;
        }
        else {
             r = powerModulo(buffor[i],e,n);
             buffor[i] = r;
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    amplitude.clear();
    freq.clear();

    fft(data);

    data = abs(data); // modul l. zespolonej

    QLineSeries *series = new QLineSeries();
    for(uint i = 0; i < data.size()/2; ++i) {
        amplitude.push_back(real(data[i])); // kopiuj do wektora czesc rzeczywista
        freq.push_back((i * header.SampleRate) / (double)data.size());
        series->append(freq[i],amplitude[i]);
    }
    QChart *chart = new QChart();
       chart->legend()->hide();
       chart->addSeries(series);
       chart->createDefaultAxes();
       chart->setTitle("FFT");
       Plot* plot = new Plot(chart);
       plot->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    file = fopen("nowy.wav", "wb");
    fwrite(&header, sizeof(header), 1, file);
    short tmp;
    for(uint i = 0; i < buffor.size(); ++i) {
        tmp=buffor[i];
        fwrite(&tmp,sizeof(short),1,file);
    }
    fclose(file);
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("/home/tomasz/build-emedia2-Desktop_Qt_5_11_0_GCC_64bit-Debug/nowy.wav"));
    player->setVolume(50);
    player->play();
}

void MainWindow::on_pushButton_7_clicked()
{
    amplitude.clear();
    unsigned long long p = 1237;
    unsigned long long q = 17;
    unsigned long long n = p * q;
    unsigned long long fi = (p-1) * (q-1);

    // zeby klucz publiczny nie był zbyt mały zacznij szukac od polowy n
    uint e = n/2;

    while (gcd(fi,e) > 1 && e < n) {
        ++e;
    }
    std::cout << "ENC: " << std::endl;

    int d = egcd(e,fi);
    unsigned long long oper;
    short r;
    for(uint i = 0; i < buffor.size(); ++i) {
        if(buffor[i] < 0) {
             r = -1 * buffor[i];
             r = powerModulo(r,d,n);
             buffor[i] = -1 * r;
        }
        else {
             buffor[i] = powerModulo(buffor[i],d,n);
        }
    }

}
