#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mygraphicsview.h"
#include "mysliderdialog.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(QSize(800, 600));
    setMinimumSize(200, 200);
    setWindowTitle(tr("EMS"));
    //setMaximumSize(QApplication::desktop()->screenGeometry().width(), QApplication::desktop()->screenGeometry().height());

    scene = new QGraphicsScene(this);

    view = new MyGraphicsView(this);
    view->setScene(scene);
    dialog = new MySliderDialog(this);

    gridLayout = new QGridLayout(ui->centralWidget);
    gridLayout->addWidget(view);

    updateStatusBar(tr("Programm started."));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction("recent"+i, this);
        recentFileActs[i]->setVisible(true);
        connect(recentFileActs[i], SIGNAL(triggered()),
                     this, SLOT(openRecentFile()));
    }
    ui->menuFile->addSeparator();

    for (int i = 0; i < MaxRecentFiles; ++i){
        ui->menuFile->addAction(recentFileActs[i]);
    }

    QString translatorName = "Translation_";
    translatorName.append(QLocale::system().name());


    bool successfullyLoaded = translatorGerman.load(translatorName, qApp->applicationDirPath());
    if(successfullyLoaded){
        qDebug() << "loaded translation successful.";
    }

    updateRecentFileActions();

    connect(dialog, SIGNAL(rValueChanged(int)), view, SLOT(rValue(int)));
    connect(dialog, SIGNAL(gValueChanged(int)), view, SLOT(gValue(int)));
    connect(dialog, SIGNAL(bValueChanged(int)), view, SLOT(bValue(int)));
    connect(dialog, SIGNAL(yValueChanged(int)), view, SLOT(yValue(int)));
    connect(dialog, SIGNAL(uValueChanged(int)), view, SLOT(uValue(int)));
    connect(dialog, SIGNAL(vValueChanged(int)), view, SLOT(vValue(int)));
    connect(dialog, SIGNAL(previewChanged(bool)), view, SLOT(preview(bool)));
    connect(dialog, SIGNAL(buttonAccepted()), view, SLOT(okayButton()));
    connect(dialog, SIGNAL(buttonRejected()), view, SLOT(rejectedButton()));
    connect(dialog, SIGNAL(rgbRadio(bool)), this, SLOT(rgbRadio(bool)));
    connect(dialog, SIGNAL(yuvRadio(bool)), this, SLOT(yuvRadio(bool)));

    readSettings();
}

/*
 * --- SLOTS ---
*/

void MainWindow::open(){
    if(view->isEdited()){
        int returnValue = saveCancelDiscard();
        if(returnValue == QMessageBox::Cancel){
            return;
        }
    }

    dialog->reset();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images")+" (*.png *.bmp *.jpg *.jpeg);;"+tr("All Files")+" (*.*)");
    originalFileName = fileName;
    if(!fileName.isEmpty()){
        QImage image(fileName);
        if(image.isNull()){
            QMessageBox::information(this, tr("Image Viewer"), tr("Could not open the file %1.").arg(fileName));
            updateStatusBar(tr("Could not open File."));
        } else {
            view->setImage(QPixmap::fromImage(image));

            setWindowTitle(fileName);
            updateStatusBar(tr("Image successfully loaded."));

            //recent files
            setCurrentFile(fileName);
            view->setEdited(false);
        }
    }
}
void MainWindow::save(){
    QString fileFormat = getFileFormat(originalFileName);
    if(!originalFileName.isEmpty()){
        QPixmap pixmap = view->imageItem->pixmap();
        pixmap.save(originalFileName, fileFormat.toStdString().c_str());
        updateStatusBar(tr("Image successfully saved."));

        //recent files
        setCurrentFile(originalFileName);
        view->setEdited(false);
    } else {
        updateStatusBar(tr("Image could not be saved."));
    }
}
void MainWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("PNG")+" (*.png);;"+tr("JPEG")+" (*.jpg *.jpeg);;"+tr("BMP")+" (*.bmp)");
    QString fileFormat = getFileFormat(fileName);
    if(!fileName.isEmpty()){
        QPixmap pixmap = view->imageItem->pixmap();
        pixmap.save(fileName, fileFormat.toStdString().c_str());
        updateStatusBar(tr("Image successfully saved."));

        //recent fils
        setCurrentFile(fileName);
        view->setEdited(false);
    } else {
        updateStatusBar(tr("Image could not be saved."));
    }
}
void MainWindow::zoomIn(){
    view->zoomIn(1.25);
}
void MainWindow::zoomOut(){
    view->zoomOut(0.8);
}
void MainWindow::normalSize(){
    view->resetTransform();
    updateStatusBar(tr("Image restored to original size."));
}
void MainWindow::openDialog(){
    dialog->show();
}
void MainWindow::about(){
    QMessageBox::about(this, tr("About Belegarbeit"), tr("Members: \nMarcel Schwittlick (s0529494) \nAlexander Marten (s0527574)\nDennis H�gler (s05232338)"));
}
void MainWindow::aboutQt(){
    QMessageBox::aboutQt(this);
}
void MainWindow::rValue(int r)
{
    view->rValue(r);
}
void MainWindow::gValue(int g)
{
    view->gValue(g);
}
void MainWindow::bValue(int b)
{
    view->bValue(b);
}
void MainWindow::rgbRadio(bool rgb){
    dialog->enableRGBSliders(rgb);
    if(rgb){
        view->selectedMode = 4;
    } else {
        view->selectedMode = 5;
    }
}
void MainWindow::yuvRadio(bool yuv){
    //delete me
}
void MainWindow::copyImage(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(view->imageItem->pixmap().toImage());
    updateStatusBar(tr("Copied image to clipboard."));
}
void MainWindow::pasteImage(){
    if(view->isEdited()){
        int returnValue = saveCancelDiscard();
        if(returnValue == QMessageBox::Cancel){
            return;
        }
    }

    dialog->reset();

    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    //checks if the clipboard contains an url to a file and sets it
    //in the view if it is an image
    if(mimeData->hasUrls()){
        QImage image(mimeData->urls().first().toLocalFile());
        if(!image.isNull()){
            setWindowTitle(mimeData->urls().first().toString());
            view->setImage(QPixmap::fromImage(image));
            view->setEdited(false);
            updateStatusBar(tr("Pasting from clipboard succeded."));
        } else {
            updateStatusBar(tr("Pasting from clipboard failed."));
        }
    }

    //checks if an image is in the clipboard and sets in the view
    if(clipboard->image().isNull()){
        updateStatusBar(tr("Pasting from clipboard failed."));
    } else {
        view->setImage(QPixmap::fromImage(clipboard->image()));
        view->setEdited(false);
        updateStatusBar(tr("Pasting from clipboard succeded."));
        setWindowTitle("*");
    }
}

void MainWindow::loadFile(const QString &fileName){
    if(view->isEdited()){
        int returnValue = saveCancelDiscard();
        if(returnValue == QMessageBox::Cancel){
            return;
        }
    }

    dialog->reset();

    originalFileName = fileName;
    if(!fileName.isEmpty()){
        QImage image(fileName);
        if(image.isNull()){
            QMessageBox::information(this, tr("Image Viewer"), tr("Could not open the file %1.").arg(fileName));
            updateStatusBar(tr("Could not open File."));
        } else {
            view->setImage(QPixmap::fromImage(image));

            setWindowTitle(fileName);
            updateStatusBar(tr("Image successfully loaded."));

            //recent files

            setCurrentFile(fileName);
            view->setEdited(false);
        }
    }
}

void MainWindow::readSettings(){
    QSettings settings( "ai.bachelor.htw-berlin.de", "EMS-Beleg");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::writeSettings(){
    QSettings settings("ai.bachelor.htw-berlin.de", "EMS-Beleg");
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("isMinimized", isMinimized());
    settings.setValue("isMaximized", isMaximized());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();
    if(view->isEdited()) {
        int returnValue = saveCancelDiscard();
        switch(returnValue) {
        case QMessageBox::Save:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        case QMessageBox::Discard:
            event->accept();
        }
    }
}

int MainWindow::saveCancelDiscard()
{
    QMessageBox msgBox;
     msgBox.setText("The image has been edited.");
     msgBox.setInformativeText("Do you want to save your changes?");
     msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Save);
     int ret = msgBox.exec();
     qDebug() << ret;
     if(ret == QMessageBox::Save){
         saveAs();
         dialog->reset();
         return QMessageBox::Save;
     } else if(ret == QMessageBox::Discard){
         dialog->reset();
         return QMessageBox::Discard;
     } else if(ret == QMessageBox::Cancel) {
         return QMessageBox::Cancel;
     } else {
         return 0;
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openRecentFile()
{
     QAction *action = qobject_cast<QAction *>(sender());
     if (action) {
         loadFile(action->data().toString());
     }
}

void MainWindow::setCurrentFile(const QString &fileName)
 {
     curFile = fileName;
     dialog->reset();

     QSettings settings("ai.bachelor.htw-berlin.de", "EMS-Beleg");
     settings.beginGroup("RecentFiles");
     QStringList files = settings.value("recentFileList").toStringList();
     files.removeAll(fileName);
     files.prepend(fileName);
     while (files.size() > MaxRecentFiles)
         files.removeLast();

     settings.setValue("recentFileList", files);
     settings.endGroup();

     updateRecentFileActions();
 }

void MainWindow::updateRecentFileActions()
 {
     QSettings settings("ai.bachelor.htw-berlin.de", "EMS-Beleg");
     settings.beginGroup("RecentFiles");
     QStringList files = settings.value("recentFileList").toStringList();
     settings.endGroup();

     int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
     for (int i = 0; i < numRecentFiles; ++i) {
         QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
         recentFileActs[i]->setText(text);
         recentFileActs[i]->setData(files[i]);
         recentFileActs[i]->setVisible(true);
     }
     for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
         recentFileActs[j]->setVisible(false);
 }

 QString MainWindow::strippedName(const QString &fullFileName)
 {
     return QFileInfo(fullFileName).fileName();
 }

 void MainWindow::languageEnglish(){
    qApp->removeTranslator(&translatorGerman);
 }

 void MainWindow::languageGerman(){
    qApp->installTranslator(&translatorGerman);
 }

 void MainWindow::changeEvent(QEvent *event){
     if(event->type() == QEvent::LanguageChange){
         ui->retranslateUi(this);
         qDebug() << "changevent";
     }
 }


 QString MainWindow::getFileFormat(QString strImageFileName){
     QString returnString = "";
     QStringList splittedStrList = strImageFileName.split(".");
     if( ((splittedStrList.size())-1) >= 0) {
         returnString = splittedStrList[((splittedStrList.size())-1)];
     }
     return returnString;
 }

 void MainWindow::updateStatusBar(QString string)
 {
     statusBar()->showMessage(string);
 }
