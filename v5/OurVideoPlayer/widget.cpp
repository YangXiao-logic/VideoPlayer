#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    playernumbers = 0;
    playerindex = 0;
    player = new ThePlayer;
    player->setVideoOutput(ui->videoWidget);
    playertitle= "Video name";
    //Set video progress bar
    connect(player,&QMediaPlayer::durationChanged,ui->playerslider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,ui->playerslider,&QSlider::setValue);
    connect(ui->playerslider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);



    //Set volume progress bar
    connect(player,&QMediaPlayer::volumeChanged,ui->volumeslider,&QSlider::setValue);
    connect(ui->volumeslider,&QSlider::sliderMoved,player,&QMediaPlayer::setVolume);
    connect (player,SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (volumeStateChanged()));

    //Detect video status and change button icon
    QTimer *mTimer = new QTimer(nullptr);
    mTimer->setInterval(100);
    mTimer->start();
    connect( mTimer, SIGNAL (timeout()), SLOT(volumeStateChanged()));
    connect (mTimer, SIGNAL (timeout()),SLOT (updateduration()));
    connect (player, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playerStateChanged()));

}

Widget::~Widget(){
    delete ui;
}

//slots
void Widget::on_open_clicked(){
    QString array = QFileDialog::getExistingDirectory(this,"need to play file","C:/Users/loulo/Desktop/19 UI/videos");
    if(array.length() <= 0)
        return;
    std::string dirName = array.toStdString();
    getVideo(dirName);
    creatbuttonList();
    setVideoTitle(playerindex);
}


void Widget::on_pause_clicked(){
    if(playernumbers == 0){
        return;
    }
    switch (player->state())
    {
        case QMediaPlayer::State::PausedState:
            player->play(); // starting playing again...
            ui->pause->setIcon(QIcon(":/icon/pause.png"));
            break;
        default:
            player->pause();
            ui->pause->setIcon(QIcon(":/icon/start.png"));
            break;
    }
}


void Widget::on_fullscreen_clicked(){
    ui->videoWidget->setFullScreen(true);
}


void Widget::on_mute_clicked(){
    switch (player->volume())
    {
        case 0:
            player->setVolume(20);
            ui->mute->setIcon(QIcon(":/icon/volumeHigh.png"));
            break;
        default:
            player->setVolume(0);
            ui->mute->setIcon(QIcon(":/icon/volumeDisable.png"));
            break;
    }
}


void Widget::on_backward_clicked(){
    if(playernumbers == 0){
        return;
    }
    qint64 backward = player->position();
    backward = backward - 5000;
    player->setPosition(backward);
}


void Widget::on_forward_clicked(){
    if(playernumbers == 0){
        return;
    }
    qint64 forward = player->position();
    forward = forward + 5000;
    player->setPosition(forward);
}


void Widget::on_speed_clicked(){
    if(playernumbers == 0){
        return;
    }
    player->setPlaybackRate(2);
    QMessageBox::question(
        nullptr,
        QString("Tomeo"),
        QString("doSetRate: Audio device or filter does not support rate. "
                "Falling back to previous value."),
        QMessageBox::Yes);
}


void Widget::playerStateChanged (){
    switch (player->state()) {
        case QMediaPlayer::State::PausedState:
            ui->pause->setIcon(QIcon(":/icon/start.png"));
            break;
        default:
            ui->pause->setIcon(QIcon(":/icon/pause.png"));
            break;
    }
    switch (player->volume()) {
        case 0:
            ui->mute->setIcon(QIcon(":/icon/volumeDisable.png"));
            break;
        default:
            ui->mute->setIcon(QIcon(":/icon/volumeHigh.png"));
            break;
    }
}


void Widget::volumeStateChanged (){
    switch (player->volume()) {
        case 0:
            ui->mute->setIcon(QIcon(":/icon/volumeDisable.png"));
            break;
        default:
            ui->mute->setIcon(QIcon(":/icon/volumeHigh.png"));
            break;
    }
}


void Widget::on_tag_clicked() {
    QMessageBox::question(
        nullptr,
        QString("Tomeo"),
        QString("Used to tag the video to classify it. Not yet implemented."),
        QMessageBox::Yes);
}


void Widget::on_user_clicked() {
    QMessageBox::question(
        nullptr,
        QString("Tomeo"),
        QString("used for user registration/login, after login, "
                "you will enter the user interface to manage user information. Not yet implemented."),
        QMessageBox::Yes);
}


void Widget::on_search_clicked() {
    QMessageBox::question(
        nullptr,
        QString("Tomeo"),
        QString("Click the search box and enter text to search for the video. Not yet implemented."),
        QMessageBox::Yes);
}


void Widget::on_share_clicked() {
    QMessageBox::question(
        nullptr,
        QString("Tomeo"),
        QString("Click to share the current video, and you can choose to share with. Not yet implemented."),
        QMessageBox::Yes);
}


void Widget::on_menu_clicked() {
    QMessageBox::question(
        nullptr,
        QString("Tomeo"),
        QString("Enter the menu interface. Not yet implemented."),
        QMessageBox::Yes);
}



void Widget::on_next_clicked() {
    if(playernumbers == 0){
        return;
    }
    playerindex= playerindex+1;
    TheButtonInfo* button = player->getButtons()->at(playerindex)->info;
    player->jumpTo(button);
    setVideoTitle(playerindex);
    if(playerindex == playernumbers-1){
        playerindex = -1;
    }
}

void Widget::getbuttonindex(int index){
    QString qString;
    playerindex=index;
    qDebug()<<qString.number(playerindex);

}

void Widget::setVideoTitle(int index){
    QString title = player->getButtons()->at(index)->info->videoname;
    ui->videoname->setText(title);
    playertitle=title;
    qDebug()<<playertitle;

}

void Widget::updateduration(){
    qint64 time1 = player->position()/1000;
    qint64 time2 = player->duration()/1000;
    QString Str1 = QString::number(time1);
    Str1 = Str1 + "/";
    Str1 = "00:" + Str1;
    QString Str2 = QString::number(time2);
    Str2 = "00:" + Str2;
    Str1.append(Str2);
    ui->time->setText(Str1);
}

void Widget::on_upload_clicked()
{
        QMessageBox::question(
            nullptr,
            QString("Tomeo"),
            QString("Click to upload the video, drag the file into the box to upload it. Not yet implemented."),
            QMessageBox::Yes);

}

//end slots


//function
std::vector<TheButtonInfo> Widget::getInfoIn (std::string loc) {
    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    //get dir name
    QString dirname=QString::fromStdString(loc);
    qDebug()<<dirname;
    int dirlength = dirname.length();
    qDebug()<<dirlength;

    while (it.hasNext()) {
        QString f = it.next();
            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv")) { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            QString videoname = f.right(f .length() - dirlength - 1);
            qDebug()<<videoname;
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico , videoname));
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }
    qDebug()<<out.size();
    playernumbers = out.size();
    return out;
}

void Widget::getVideo(const std::string dirName) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // collect all the videos in the folder
    videoList.clear();
    videoList = getInfoIn(dirName);

    if (videoList.size() == 0) {
        const int result = QMessageBox::question(
                    nullptr,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result ) {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        return;
    }
}

void Widget::creatbuttonList() {
    // the QMediaPlayer which controls the playback
    QVBoxLayout *layout = new QVBoxLayout();
    ui->buttonWidget->setLayout(layout);

    // create the six buttons
    for ( int i = 0; i < playernumbers; i++ ) {
        TheButton *button = new TheButton(ui->buttonWidget);
        button->index = i;
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        button->connect(button, SIGNAL(returnindex(int)), this, SLOT (getbuttonindex(int)));
        button->connect (button, SIGNAL(returnindex(int)), this, SLOT (setVideoTitle(int)));
        buttonList.push_back(button);
        button->setFlat(true);
        button->setFocusPolicy(Qt::NoFocus);
        layout->addWidget(button);
        button->init(&videoList.at(i));
    }
    player->setContent(&buttonList, &videoList);
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        on_pause_clicked();
        qDebug() << "space";
    }else if (event->key() == Qt::Key_Left) {
        qint64 backward = player->position();
        backward = backward - 5000;
        player->setPosition(backward);
        qDebug() << "left1";
    }else if (event->key() == Qt::Key_Right) {
        qint64 forward = player->position();
        forward = forward + 5000;
        player->setPosition(forward);
        qDebug() << "right1";
    }else if (event->key() == Qt::Key_Up) {
        int volumeplus = player->volume();
        volumeplus = volumeplus + 10;
        player->setVolume(volumeplus);
        qDebug() << "up";
    }else if (event->key() == Qt::Key_Down) {
        int volumeless = player->volume();
        volumeless = volumeless - 10;
        player->setVolume(volumeless);
        qDebug() << "down";
    }
}



//end function


