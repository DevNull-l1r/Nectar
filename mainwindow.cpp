#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>
#include <QIcon>
#include <QUrl>
#include <X11/Xlib.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("EXE Executor // Home");
    resize(640, 520);

    QString appDir = QCoreApplication::applicationDirPath();
    QString logoPath = QDir(appDir).filePath("../resources/logo.png");

    QIcon appIcon(logoPath);
    if (!appIcon.isNull()) {
        setWindowIcon(appIcon);
    }

    setStyleSheet(
        "QMainWindow { background-color: #0B0C10; }"
        "QLabel { color: #C5C6C7; font-size: 13px; font-family: 'Courier New', monospace; }"
        "QLineEdit { background-color: #1F2833; color: #66FCF1; border: 2px solid #45A29E; "
        "            border-radius: 4px; padding: 12px; font-size: 14px; font-weight: bold; }"
        "QPushButton { background-color: #1F2833; color: #45A29E; border: 2px solid #45A29E; "
        "               border-radius: 4px; padding: 12px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45A29E; color: #0B0C10; }"
        "QPushButton#LaunchBtn { background-color: #0B0C10; color: #66FCF1; border: 2px solid #66FCF1; font-size: 16px; }"
        "QPushButton#LaunchBtn:hover { background-color: #66FCF1; color: #0B0C10; }"
        "QCheckBox { color: #45A29E; font-family: 'Courier New', monospace; font-size: 12px; font-weight: bold; }"
        "QCheckBox::indicator { width: 14px; height: 14px; border: 2px solid #45A29E; background-color: #1F2833; }"
        "QCheckBox::indicator:checked { background-color: #66FCF1; border: 2px solid #66FCF1; }"
        "QPlainTextEdit { background-color: #050508; color: #33FF33; border: 1px solid #1F2833; "
        "                 font-family: 'DejaVu Sans Mono', monospace; font-size: 11px; padding: 8px; }"
        "QProgressBar { border: 1px solid #1F2833; border-radius: 4px; text-align: center; color: #FFFFFF; "
        "               background-color: #121214; font-weight: bold; font-size: 11px; height: 16px; }"
        "QProgressBar::chunk { background-color: #45A29E; }"
    );

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 35, 40, 25);
    mainLayout->setSpacing(15);

    QVBoxLayout *titleBlock = new QVBoxLayout();
    titleBlock->setSpacing(6);
    QLabel *titleLabel = new QLabel("EXE EXECUTOR", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #66FCF1; letter-spacing: 3px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *authorLabel = new QLabel("by Null", this);
    authorLabel->setStyleSheet("font-size: 14px; color: #45A29E; font-weight: bold;");
    authorLabel->setAlignment(Qt::AlignCenter);
    titleBlock->addWidget(titleLabel);
    titleBlock->addWidget(authorLabel);

    QHBoxLayout *fileRowLayout = new QHBoxLayout();
    pathInput = new QLineEdit(this);
    pathInput->setPlaceholderText("Select or paste a Windows application file path (.exe) ...");
    browseButton = new QPushButton("BROWSE", this);
    fileRowLayout->addWidget(pathInput, 4); 
    fileRowLayout->addWidget(browseButton, 1); 

    adminCheckbox = new QCheckBox(" Run with administrative privileges (Recommended)", this);
    adminCheckbox->setChecked(true); 

    launchButton = new QPushButton("LAUNCH APPLICATION", this);
    launchButton->setObjectName("LaunchBtn"); 

    miniTerminal = new QPlainTextEdit(this);
    miniTerminal->setReadOnly(true);
    miniTerminal->appendPlainText("System status: Ready. Please select a target application.");

    loadingBar = new QProgressBar(this);
    loadingBar->setRange(0, 100);
    loadingBar->setValue(0);

    QHBoxLayout *footerLayout = new QHBoxLayout();
    statusLabel = new QLabel("[ Status: Standby ]", this);
    statusLabel->setStyleSheet("color: #45A29E; font-size: 11px; font-weight: bold;");
    
    musicLabel = new QLabel("", this);
    musicLabel->setStyleSheet("color: #45A29E; font-size: 11px; font-style: italic;");
    musicLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    footerLayout->addWidget(statusLabel);
    footerLayout->addWidget(musicLabel);

    mainLayout->addLayout(titleBlock);
    mainLayout->addLayout(fileRowLayout);
    mainLayout->addWidget(adminCheckbox); 
    mainLayout->addWidget(launchButton);
    mainLayout->addWidget(miniTerminal);
    mainLayout->addWidget(loadingBar);
    mainLayout->addLayout(footerLayout);

    setCentralWidget(centralWidget);

    connect(browseButton, &QPushButton::clicked, this, &MainWindow::handleBrowse);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::handleLaunch);

    wineProcess = new QProcess(this);
    connect(wineProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::readTerminalOutput);

    windowTrackerTimer = new QTimer(this);
    connect(windowTrackerTimer, &QTimer::timeout, this, &MainWindow::checkActiveWindows);

    initMenuMusic();
}

void MainWindow::initMenuMusic() {
    QString musicPath = "/app/share/exe-executor/themes/menu_theme.ogg";
    
    if (!QFileInfo::exists(musicPath)){
    // Holy typo on the last commit 😭
        musicPath = "../resources/menu_theme.ogg";
    }

    if (!QFileInfo::exists(musicPath)) {
        statusLabel->setText("[ Error: Music file not found; silent mode enabled ]");
        return; 
    }
    musicLabel->setText("Music by Posifurg");

    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile(musicPath));
    
    mediaPlayer->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(1.0); 

    mediaPlayer->play();
}

void MainWindow::handleBrowse() {
    QString fileSelected = QFileDialog::getOpenFileName(this, "Select Windows Application", QDir::homePath(), "Applications (*.exe)");
    if (!fileSelected.isEmpty()) {
        pathInput->setText(fileSelected);
        statusLabel->setText("[ File loaded ]");
        miniTerminal->appendPlainText(QString("Selected application: %1").arg(fileSelected));
    }
}

void MainWindow::readTerminalOutput() {
    QByteArray blendedOutput = wineProcess->readAllStandardOutput();
    if (!blendedOutput.isEmpty()) {
        miniTerminal->appendPlainText(QString::fromUtf8(blendedOutput).trimmed());
    }
}

void MainWindow::checkActiveWindows() {
    Display *display = XOpenDisplay(NULL);
    if (!display) return;

    Window root = DefaultRootWindow(display);
    Window root_return, parent_return, *children;
    unsigned int num_children;

    if (XQueryTree(display, root, &root_return, &parent_return, &children, &num_children)) {
        if (num_children > 0) {
            loadingBar->setValue(100);
            windowTrackerTimer->stop();
            
            if (mediaPlayer) {
                mediaPlayer->pause();
            }
        }
        XFree(children);
    }
    XCloseDisplay(display);
}

void MainWindow::handleLaunch() {
    QString exePath = pathInput->text().trimmed();
    if (exePath.isEmpty() || !QFileInfo::exists(exePath) || !exePath.endsWith(".exe", Qt::CaseInsensitive)) {
        statusLabel->setText("[ Error: Invalid file selection ]");
        miniTerminal->appendPlainText("Error: Please select a valid Windows file ending in .exe");
        return;
    }

    statusLabel->setText("[ Status: Running application... ]");
    simulatedProgress = 10;
    loadingBar->setValue(simulatedProgress);
    windowTrackerTimer->start(100); 

    QStringList arguments;
    if (adminCheckbox->isChecked()) {
        miniTerminal->appendPlainText("Security Status: Launching with administrative privileges.");
        arguments << exePath << "--no-sandbox" << "--disable-gpu";
    } else {
        miniTerminal->appendPlainText("Security Status: Launching as a restricted standard user.");
        arguments << "runas" << "/trustlevel:0x20000" << exePath << "--no-sandbox" << "--disable-gpu";
    }

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("DXVK_ASYNC", "1");
    env.insert("WINE_ALLOW_WINDOW_DPISCALING", "1");
    env.insert("WINEDLLOVERRIDES", "dxgi=n,b;d3d11=n,b;d3d10core=n,b;d3d9=n,b");

    wineProcess->setProcessEnvironment(env);
    wineProcess->setProcessChannelMode(QProcess::MergedChannels);
    wineProcess->start("wine", arguments);

    connect(wineProcess, &QProcess::finished, this, [this](int exitCode) {
        windowTrackerTimer->stop();
        loadingBar->setValue(100);
        
        if (mediaPlayer) {
            mediaPlayer->play();
        }
        
        if (exitCode == 0) {
            statusLabel->setText("[ Status: Application closed successfully ]");
            miniTerminal->appendPlainText("System status: Runtime environment closed safely.");
        } else {
            statusLabel->setText("[ Error: Application closed unexpectedly ]");
            miniTerminal->appendPlainText(QString("System warning: Application exited with error code: %1").arg(exitCode));
        }
    });
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
    event->accept();
}

MainWindow::~MainWindow() {}

