#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

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
    void on_openDir_Btn_clicked();

    void on_shuffle_Btn_clicked();

    void on_save_Btn_clicked();

private:
    Ui::MainWindow *ui;
    QStringList songList;            // Files with directory prefix removed
    QStringList songListWithDir;     // Files with directory prefix
    QStringList shuffledList;        // Shuffled files with key prefix
    QString ogPath;                  // Source path as a string
    QVector<QString> ogNameVector;   // List of unsorted names, no prefix
    QVector<QString> nameWithCode;   // List of unsorted names with code, no prefix
    QVector<QString> shuffledVector; // List of sorted names, no prefix
};

#endif // MAINWINDOW_H
