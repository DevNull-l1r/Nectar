#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QProcess>
#include <QTimer>
#include <QCloseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleLaunch();
    void handleBrowse();
    void readTerminalOutput();
    void checkActiveWindows();

private:
    QLineEdit *pathInput;
    QPushButton *browseButton;
    QPushButton *launchButton;
    QCheckBox *adminCheckbox;
    QLabel *statusLabel;
    QLabel *musicLabel;
    QProgressBar *loadingBar;
    QPlainTextEdit *miniTerminal;
    QProcess *wineProcess;
    QTimer *windowTrackerTimer;
    int simulatedProgress;
    
    QMediaPlayer *mediaPlayer = nullptr;
    QAudioOutput *audioOutput = nullptr;
    
    void initMenuMusic();
};


