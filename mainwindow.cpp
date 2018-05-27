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


int gcd(unsigned long long a,unsigned long long int b) {
    int tmp;
    while (b != 0)  {
      tmp = b;
      b = a % b;
      a = tmp;
    }
    return a;
}

/* Rozszerzony Algorytm Euklidesa.
https://en.wikibooks.org/wiki/Algorithm_Implementation/Mathematics/Extended_Euclidean_algorithm
*/
int egcd(unsigned long long e, unsigned long long fi) {
    long d = 0;
    long x1 = 0;
    long x2 = 1;
    long y1 = 1;
    long temp = fi;

    while (e > 0) {
        int temp1 = temp/e;
        int temp2 = temp - temp1 * e;
        temp = e;
        e = temp2;

        int x = x2- temp1* x1;
        int y = d - temp1 * y1;

        x2 = x1;
        x1 = x;
        d = y1;
        y1 = y;
    }
    if (temp == 1) {
        return d + fi;
    }
    else return 0;
}

// zwraca wartość (base^exp) % mod
unsigned long long powerModulo(unsigned long long a, unsigned long long b, unsigned long long c) {
    unsigned long long n, *pows, *indexes, indexCounter = 0, searchBit = 1, partialMul = 1;
    n = floor(log2(b)) + 1;
    pows = new unsigned long long[n];
    indexes = new unsigned long long[n];
    pows[0] = a % c;
    for (int i = 1; i < n; i++) {
        pows[i] = (pows[i - 1] * pows[i - 1]) % c;
    }


    for (int i = 0; i < n; i++) {
        int index = b & searchBit;
        if (index != 0) {
            indexes[indexCounter] = floor(log2(index));
            indexCounter++;
        }
        searchBit = searchBit << 1;
    }

    for (int i = 0; i < indexCounter; i++) {
        partialMul = (partialMul * pows[indexes[i]]) % c;
    }
    delete[] pows;
    delete[] indexes;
    return partialMul % c;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data.resize(BUFSIZE);
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
    fread(buffor, sizeof(buffor), 1, file);
    fclose(file);
    filepath = filename;
}



void MainWindow::on_pushButton_2_clicked()
{
    amplitude.clear();
    freq.clear();

    for(uint i = 0; i < BUFSIZE; ++i) {
        int sample = buffor[i];
        double nSample = sample; // normalizacja danych do zakresu (-1,1)
        data[i] = Complex(nSample);      // dodaj do tablicy liczb zespolonych
    }

 //   fft(data);

   // data = abs(data); // modul l. zespolonej

    QLineSeries *series = new QLineSeries();
    for(uint i = 0; i < data.size()/2; ++i) {
        amplitude.push_back(real(data[i])); // kopiuj do wektora czesc rzeczywista
        freq.push_back((i * header.SampleRate) / (double)BUFSIZE);
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
    std::cout << "ORG: " << std::endl;
    for(uint i = 0; i < BUFSIZE; ++i) {
        std::cout << buffor[i] << std::endl;
    }

    std::cout << "Zaszyfrowane: " << std::endl;
    short  r;
    for(uint i = 0; i < BUFSIZE; ++i) {
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
         std::cout << buffor[i] << std::endl;
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
        freq.push_back((i * header.SampleRate) / (double)BUFSIZE);
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
    int tmp;
    for(uint i = 0; i < amplitude.size(); ++i) {
        tmp=amplitude[i];
        fwrite(&tmp,sizeof(int),1,file);
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
    for(uint i = 0; i < BUFSIZE; ++i) {
        if(buffor[i] < 0) {
             r = -1 * buffor[i];
             r = powerModulo(r,d,n);
             r = -1 * r;
        }
        else {
             r = powerModulo(buffor[i],d,n);
        }
        amplitude.push_back(r);      // dodaj do tablicy liczb zespolonych
        std::cout <<  r << std::endl;
    }

}
