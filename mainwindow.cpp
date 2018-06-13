#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStringListModel>
#include <QDir>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MP3 Randomizer");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openDir_Btn_clicked()
{
    int sLast;          // Last index of
    QString string;     // Temp

    songList = QFileDialog::getOpenFileNames(this, tr ("Select Files"),
                                               QDir::homePath(), //"C://"
                                               "Music files (*.mp3)");

    songListWithDir = songList;       //Make a copy for later use. Leave path prefix
    ogNameVector.clear();
    nameWithCode.clear();

    // Remove path prefix - store into songList
    for (int i = 0; i < songList.size(); ++i)
    {
        string = songList[i];
        sLast = string.lastIndexOf('/');
        string.remove(0, sLast+1);
        nameWithCode.push_back(string);

        if(string.contains('~'))
        {
            string.remove(0, string.indexOf('~')+2);
        }
        songList[i] = string;
        ogNameVector.push_back(string);
    }

    // Set the model
    QStringListModel *model = new QStringListModel();
    model->setStringList(songList);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(false);
    ui->rightLabel->clear();
    if(!songListWithDir.isEmpty())
    {
        ogPath = songListWithDir[0].left(sLast);
        ui->leftLabel->setText(ogPath);
    }

    // Clear right table
    QStringList empty;
    QStringListModel *model1 = new QStringListModel();
    model1->setStringList(empty);
    ui->listView_2->setModel(model1);
    ui->listView_2->setEditTriggers(false);
}

void MainWindow::on_shuffle_Btn_clicked()
{
    QString temp;   // Temp string
    std::random_shuffle(songList.begin(), songList.end());
    shuffledList.clear();
    shuffledVector.clear();

    // Append key
    for(int i = 0; i < songList.size(); ++i)
    {
        temp = songList[i];
        temp = "A" + QString::number(i) + " ~ " + temp;
        shuffledList << temp;
        shuffledVector.push_back(temp);
    }

    // Set the model
    QStringListModel *model = new QStringListModel();
    model->setStringList(shuffledList);
    ui->listView_2->setModel(model);
    ui->listView_2->setEditTriggers(false);
    if(!shuffledList.isEmpty()) ui->rightLabel->setText("Shuffled");
    ui->savedLabel->clear();
}

void MainWindow::on_save_Btn_clicked()
{
    // Get the path to save to
    QString saveDirectory = QFileDialog::getExistingDirectory(this, tr("Save to Folder"),
                                                              QDir::homePath(),
                                                              QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks);
    QString savePath = saveDirectory + '/';
    bool notSamePath = saveDirectory != ogPath;
    qDebug() << saveDirectory << ogPath;

    if(saveDirectory.isEmpty() || saveDirectory.isNull()) return;

    // Same directory. Only rename files and do not clear all .mp3 and copy files
    if(notSamePath)
    {
        // Clear directory of all .mp3 files before saving. Avoid duplicates
        QDir dir(savePath);
        dir.setNameFilters(QStringList() << "*.mp3");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    }

    // Copy from original directory to new directory and rename
    for(int i = 0; i < songListWithDir.size(); ++i)
    {
        if(notSamePath) QFile::copy(songListWithDir[i], savePath + ogNameVector[i]);

        // Rename only files with the same substring
        for(int a = 0; a < shuffledVector.size(); ++a)
        {
            if(shuffledVector[a].contains(ogNameVector[i], Qt::CaseInsensitive))
            {
                if(notSamePath)
                {
                    qDebug() << "\nog:" << savePath+ogNameVector[i];
                    qDebug() << "shuff:" << savePath+shuffledVector[a];
                    qDebug() << QFile::rename(savePath + ogNameVector[i],
                                  savePath + shuffledVector[a]);
                }
                else
                {
                    qDebug() << "\nnameCode:" << savePath+nameWithCode[i];
                    qDebug() << "shuff:" << savePath+shuffledVector[a];
                    qDebug() << QFile::rename(savePath + nameWithCode[i],
                                  savePath + shuffledVector[a]);
                }
            }
        }
    }

    qDebug() << endl;
    ui->savedLabel->setText("Saved to " + saveDirectory);
}
