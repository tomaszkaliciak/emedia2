#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

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



void MainWindow::on_openFileButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this,
                                   tr("Wybierz plik .wav",
                                   "~/"),
                                   "Music file (*.wav)");

    file = fopen(filename.toUtf8(), "rb");
    fread(&header, sizeof(header), 1, file);

    long numberOfSamples = (8 * header.Subchunk2Size) / (header.NumChannels * header.BitsPerSample);
    buffor.resize(numberOfSamples);
    data.resize(numberOfSamples);

    fread(&buffor[0], sizeof(std::vector<short>::value_type), buffor.size(),file);

    fclose(file);
    filepath = filename; //zapisz nazwę pliku, aby PLAY ORG wiedzial co zagrac
}

void MainWindow::on_plotButton_clicked() {
    amplitude.clear();
    freq.clear();

    double nSample;

    for(uint i = 0; i < buffor.size(); ++i) {
        nSample = buffor[i]/32768.0; // normalizacja danych do zakresu (-1,1)
        data[i] = Complex(nSample);  // dodaj do tablicy liczb zespolonych
    }

    fft(data);

    data = abs(data); // modul l. zespolonej

    QLineSeries *series = new QLineSeries();

    for(uint i = 0; i < data.size()/2; ++i) {
        amplitude.push_back(real(data[i])); // kopiuj do wektora czesc rzeczywista
        freq.push_back((1.0 * i * header.SampleRate)/ data.size());
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

void MainWindow::on_playOrgButton_clicked(){
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(filepath));
    player->setVolume(100);
    player->play();
}

void MainWindow::on_rsaButton_clicked() {
    QString choice = ui->rsaComboBox->currentText();
    if(choice == "Encrypt") {
        for(auto& i : buffor) {
            i = static_cast<short>(rsa.encode(i));
        }
    }
    else {
        for(auto& i : buffor) {
            i = static_cast<short>(rsa.decode(i));
        }
    }
}

void MainWindow::on_saveAndPlayButton_clicked()
{
    QString name = ui->fileNameBox->toPlainText();
    if(name=="") name = "naprzyszloscpodajnazwe.wav";

    file = fopen(name.toUtf8(), "wb");
    fwrite(&header, sizeof(header), 1, file);
    fwrite(&buffor[0], sizeof(std::vector<short>::value_type), buffor.size(),file);
    fclose(file);

    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo(name).absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
