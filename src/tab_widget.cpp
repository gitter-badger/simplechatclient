/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 ****************************************************************************/

#include "tab_widget.h"

tab_widget::tab_widget(QString param1, QWidget *parent, QTcpSocket *param2, QSettings *param3, dlg_channel_settings *param4, dlg_moderation *param5)
{
    strName = param1;
    socket = param2;
    settings = param3;
    dlgchannel_settings = param4;
    dlgmoderation = param5;

    iNickCount = 0;
    bCursorPositionChanged = false;
    strCurrentColor = "000000";

    notify = new qnotify();

    timer = new QTimer();
    timer->setInterval(1000*60*60*4); // 4h
    timer->start();

    inputline = new qinputline(this);
    inputline->setParent(this);
    inputline->setMinimumWidth(400);
    inputline->setMaxLength(300);
    inputline->show();

    splitter = new QSplitter(this);
    leftLayout = new QGridLayout();
    rightLayout = new QGridLayout();
    leftWidget = new QWidget(this);
    rightWidget = new QWidget(this);
    toolWidget = new QWidget(this);
    toolBox = new QHBoxLayout(this);
    toolBox->setAlignment(Qt::AlignLeft);

    bold = new QPushButton(this);
    bold->setFont(QFont("Times New Roman", -1, 75, false));
    bold->setText("B");
    bold->setParent(this);
    bold->setFlat(true);
    bold->setCheckable(true);
    bold->setMaximumWidth(25);
    bold->setMaximumHeight(25);
    bold->show();
    bBold = false;

    italic = new QPushButton(this);
    italic->setFont(QFont("Times New Roman", -1, -1, true));
    italic->setText("I");
    italic->setParent(this);
    italic->setFlat(true);
    italic->setCheckable(true);
    italic->setMaximumWidth(25);
    italic->setMaximumHeight(25);
    italic->show();
    bItalic = false;

    arialAct = new QAction("Arial", this);
    arialAct->setFont(QFont("Arial", -1, -1, false));
    timesAct = new QAction("Times", this);
    timesAct->setFont(QFont("Times New Roman", -1, -1, false));
    verdanaAct = new QAction("Verdana", this);
    verdanaAct->setFont(QFont("Verdana", -1, -1, false));
    tahomaAct = new QAction("Tahoma", this);
    tahomaAct->setFont(QFont("Tahoma", -1, -1, false));
    courierAct = new QAction("Courier", this);
    courierAct->setFont(QFont("Courier New", -1, -1, false));

    fontMenu = new QMenu();
    fontMenu->addAction(arialAct);
    fontMenu->addAction(timesAct);
    fontMenu->addAction(verdanaAct);
    fontMenu->addAction(tahomaAct);
    fontMenu->addAction(courierAct);

    fontfamily = new QPushButton(this);
    fontfamily->setFont(QFont("Verdana", -1, -1, false));
    fontfamily->setText("Verdana");
    fontfamily->setParent(this);
    fontfamily->setMaximumWidth(250);
    fontfamily->setMaximumHeight(25);
    fontfamily->setMenu(fontMenu);
    fontfamily->show();
    inputline->setFont(QFont("Verdana", -1, -1, false));

    size8Act = new QAction("8", this);
    size8Act->setFont(QFont("Verdana", 8, -1, false));
    size9Act = new QAction("9", this);
    size9Act->setFont(QFont("Verdana", 9, -1, false));
    size10Act = new QAction("10", this);
    size10Act->setFont(QFont("Verdana", 10, -1, false));
    size11Act = new QAction("11", this);
    size11Act->setFont(QFont("Verdana", 11, -1, false));
    size12Act = new QAction("12", this);
    size12Act->setFont(QFont("Verdana", 12, -1, false));
    size14Act = new QAction("14", this);
    size14Act->setFont(QFont("Verdana", 14, -1, false));
    size18Act = new QAction("18", this);
    size18Act->setFont(QFont("Verdana", 18, -1, false));
    size20Act = new QAction("20", this);
    size20Act->setFont(QFont("Verdana", 20, -1, false));

    sizeMenu = new QMenu();
    sizeMenu->addAction(size8Act);
    sizeMenu->addAction(size9Act);
    sizeMenu->addAction(size10Act);
    sizeMenu->addAction(size11Act);
    sizeMenu->addAction(size12Act);
    sizeMenu->addAction(size14Act);
    sizeMenu->addAction(size18Act);
    sizeMenu->addAction(size20Act);

    size = new QPushButton(this);
    size->setText("a|A");
    size->setParent(this);
    size->setMaximumWidth(250);
    size->setMaximumHeight(25);
    size->setMenu(sizeMenu);
    size->show();

    color = new QComboBox(this);
    color->setParent(this);
    color->insertItem(0, QIcon(":/3rdparty/images/000000.png"),"");
    color->insertItem(1, QIcon(":/3rdparty/images/623c00.png"),"");
    color->insertItem(2, QIcon(":/3rdparty/images/c86c00.png"),"");
    color->insertItem(3, QIcon(":/3rdparty/images/ff6500.png"),"");
    color->insertItem(4, QIcon(":/3rdparty/images/ff0000.png"),"");
    color->insertItem(5, QIcon(":/3rdparty/images/e40f0f.png"),"");
    color->insertItem(6, QIcon(":/3rdparty/images/990033.png"),"");
    color->insertItem(7, QIcon(":/3rdparty/images/8800ab.png"),"");
    color->insertItem(8, QIcon(":/3rdparty/images/ce00ff.png"),"");
    color->insertItem(9, QIcon(":/3rdparty/images/0f2ab1.png"),"");
    color->insertItem(10, QIcon(":/3rdparty/images/3030ce.png"),"");
    color->insertItem(11, QIcon(":/3rdparty/images/006699.png"),"");
    color->insertItem(12, QIcon(":/3rdparty/images/1a866e.png"),"");
    color->insertItem(13, QIcon(":/3rdparty/images/008100.png"),"");
    color->insertItem(14, QIcon(":/3rdparty/images/959595.png"),"");
    color->setIconSize(QSize(50,10));
    color->show();

    channel_settings = new QPushButton(this);
    channel_settings->setText("Ustawienia");
    channel_settings->setParent(this);
    channel_settings->show();

    moderation = new QPushButton(this);
    moderation->setText("Moderacja");
    moderation->setParent(this);
    moderation->show();

    toolBox->addWidget(bold);
    toolBox->addWidget(italic);
    toolBox->addWidget(fontfamily);
    toolBox->addWidget(color);
    toolBox->addWidget(size);
    toolBox->addWidget(channel_settings);
    toolBox->addWidget(moderation);
    toolWidget->setLayout(toolBox);

    mainLayout = new QGridLayout(this);

    nick_list = new qnicklist(socket, settings, strName);
    nick_list->setParent(this);
    nick_list->setSortingEnabled(false);
    nick_list->show();

    topic = new QLineEdit();
    topic->setParent(this);
    topic->setReadOnly(true);
    topic->setMaxLength(200);
    topic->show();

    topicButton = new QPushButton(this);
    topicButton->setText("Ustaw");
    topicButton->setParent(this);
    topicButton->show();

    topicDetails = new QLabel();
    topicDetails->setParent(this);
    topicDetails->setOpenExternalLinks(false);
    topicDetails->setAlignment(Qt::AlignLeft);
    topicDetails->show();

    nickCount = new QLabel();
    nickCount->setParent(this);
    nickCount->setOpenExternalLinks(true);
    nickCount->setAlignment(Qt::AlignCenter);
    nickCount->show();

    webLink = new QLabel();
    webLink->setParent(this);
    webLink->setOpenExternalLinks(true);
    webLink->setAlignment(Qt::AlignCenter);
    webLink->show();

    textEdit = new QTextEdit(this);
    textEdit->setParent(this);
    textEdit->setReadOnly(true);
    textEdit->show();

    nickLabel = new QLabel();
    nickLabel->setParent(this);
    nickLabel->show();

    sendButton = new QPushButton();
    sendButton->setText("Wy¶lij");
    sendButton->setParent(this);
    sendButton->setMaximumWidth(70);
    sendButton->setMaximumHeight(25);
    sendButton->show();

    moderSendButton = new QPushButton();
    moderSendButton->setText("Prowadz±cy");
    moderSendButton->setParent(this);
    moderSendButton->setMaximumWidth(100);
    moderSendButton->setMaximumHeight(25);
    moderSendButton->show();

    if (strName[0] == '#')
    {
        rightLayout->addWidget(webLink,0,0,1,1);
        rightLayout->addWidget(nickCount,1,0,1,1);
        rightLayout->addWidget(nick_list,2,0,1,1);

        leftLayout->addWidget(topic,0,0,1,4);
        leftLayout->addWidget(topicButton,0,4,1,1);
        leftLayout->addWidget(topicDetails,1,0,1,5);
        leftLayout->addWidget(textEdit,2,0,1,5);
        leftLayout->addWidget(toolWidget,3,0,1,5);
        leftLayout->addWidget(nickLabel,4,0,1,1);
        leftLayout->addWidget(inputline,4,1,1,1);
        leftLayout->addWidget(sendButton,4,2,1,1);
        leftLayout->addWidget(moderSendButton,4,3,1,1);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);
        mainLayout->addWidget(splitter);
    }
    else if (strName[0] == '^')
    {
        topic->hide();
        topicButton->hide();
        topicDetails->hide();
        nickCount->hide();
        webLink->hide();
        moderSendButton->hide();

        rightLayout->addWidget(nick_list,0,0,1,1);
        leftLayout->addWidget(textEdit,0,0,1,3);
        leftLayout->addWidget(toolWidget,1,0,1,3);
        leftLayout->addWidget(nickLabel,2,0,1,1);
        leftLayout->addWidget(inputline,2,1,1,1);
        leftLayout->addWidget(sendButton,2,2,1,1);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);
        mainLayout->addWidget(splitter);
    }
    else
    {
        topic->hide();
        topicButton->hide();
        topicDetails->hide();
        nickCount->hide();
        webLink->hide();
        nick_list->hide();
        moderSendButton->hide();

        leftLayout->addWidget(textEdit,0,0,1,3);
        leftLayout->addWidget(toolWidget,1,0,1,3);
        leftLayout->addWidget(nickLabel,2,0,1,1);
        leftLayout->addWidget(inputline,2,1,1,1);
        leftLayout->addWidget(sendButton,2,2,1,1);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);
        mainLayout->addWidget(splitter);
    }

    if (strName == "Status") channel_settings->hide();
    topicButton->hide();
    moderation->hide();
    moderSendButton->hide();
    this->setLayout(mainLayout);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputline_return_pressed()));
    QObject::connect(inputline, SIGNAL(returnPressed()), this, SLOT(inputline_return_pressed()));
    QObject::connect(topic, SIGNAL(returnPressed()), this, SLOT(topic_return_pressed()));
    QObject::connect(topicButton, SIGNAL(clicked()), this, SLOT(topic_return_pressed()));
    QObject::connect(bold, SIGNAL(clicked()), this, SLOT(bold_clicked()));
    QObject::connect(italic, SIGNAL(clicked()), this, SLOT(italic_clicked()));

    QObject::connect(arialAct, SIGNAL(triggered()), this, SLOT(arial_triggered()));
    QObject::connect(timesAct, SIGNAL(triggered()), this, SLOT(times_triggered()));
    QObject::connect(verdanaAct, SIGNAL(triggered()), this, SLOT(verdana_triggered()));
    QObject::connect(tahomaAct, SIGNAL(triggered()), this, SLOT(tahoma_triggered()));
    QObject::connect(courierAct, SIGNAL(triggered()), this, SLOT(courier_triggered()));

    QObject::connect(size8Act, SIGNAL(triggered()), this, SLOT(size8_triggered()));
    QObject::connect(size9Act, SIGNAL(triggered()), this, SLOT(size9_triggered()));
    QObject::connect(size10Act, SIGNAL(triggered()), this, SLOT(size10_triggered()));
    QObject::connect(size11Act, SIGNAL(triggered()), this, SLOT(size11_triggered()));
    QObject::connect(size12Act, SIGNAL(triggered()), this, SLOT(size12_triggered()));
    QObject::connect(size14Act, SIGNAL(triggered()), this, SLOT(size14_triggered()));
    QObject::connect(size18Act, SIGNAL(triggered()), this, SLOT(size18_triggered()));
    QObject::connect(size20Act, SIGNAL(triggered()), this, SLOT(size20_triggered()));

    QObject::connect(color, SIGNAL(currentIndexChanged(int)), this, SLOT(color_clicked(int)));

    QObject::connect(channel_settings, SIGNAL(clicked()), this, SLOT(channel_settings_clicked()));
    QObject::connect(moderation, SIGNAL(clicked()), this, SLOT(moderation_clicked()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moder_button_clicked()));

    QObject::connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursor_position_changed()));
}

tab_widget::~tab_widget()
{
    nick_flag.clear();
    nick_suffix.clear();
    nickLabel->clear();
    timer->stop();
    textEdit->clear();
    nick_list->clear();
    nicklist.clear();

    delete nick_list;
    delete inputline;
    delete notify;
}

QString tab_widget::convert_emots(QString strData)
{
    strData.replace(QRegExp("(http:|https:)//"), "\\1\\\\"); // fix http https
    strData.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
    strData.replace(QRegExp("(http:|https:)\\\\\\\\"), "\\1//"); // fix http https
    return strData;
}

void tab_widget::display_msg(QString strTime, QString strData, int iLevel)
{
    QDateTime dt = QDateTime::fromTime_t(strTime.toInt());
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    log *l = new log();
    l->save(strName, strData);

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings->value("hide_join_part").toString() == "on")
            return;
    }

    tab_widget::display_message(strData, iLevel);
}

void tab_widget::display_msg(QString strData, int iLevel)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    log *l = new log();
    l->save(strName, strData);

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings->value("hide_join_part").toString() == "on")
            return;
    }

    tab_widget::display_message(strData, iLevel);
}

void tab_widget::display_message(QString strData, int iLevel)
{
    if (bCursorPositionChanged == true)
    {
        QTextCharFormat cf = textEdit->currentCharFormat();
        textEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        textEdit->setCurrentCharFormat(cf);
        bCursorPositionChanged = false;
    }
    if (textEdit->fontUnderline() != false) textEdit->setFontUnderline(false);
    if (textEdit->textColor() != QColor(0, 0, 0, 255)) textEdit->setTextColor(QColor(0, 0, 0, 255));
    if (textEdit->alignment() != Qt::AlignLeft) textEdit->setAlignment(Qt::AlignLeft);
    if (textEdit->fontFamily() != "Verdana") textEdit->setFontFamily("Verdana");
    if (textEdit->fontItalic() == true) textEdit->setFontItalic(false);
    if (textEdit->fontWeight() == 75) textEdit->setFontWeight(50);
    strData+="\n";
    textEdit->insertHtml("<p>");
    bool bHilight = false;
    bool bLevel = false;

    // if /me remove time/action
    if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
    {
        strData = strData.right(strData.length() - 11);
        if (strData.indexOf("ACTION ") != -1) strData = strData.replace("ACTION ", QString::null);
        if (strData.indexOf("<") != -1) strData = strData.remove(strData.indexOf("<"),1);
        if (strData.indexOf(">") != -1) strData = strData.remove(strData.indexOf(">"),1);
    }

    for (int i = 0; i < strData.length(); i++)
    {
        // colors
        if ((i == 0) && (bLevel == false))
        {
            if (iLevel == 8) // hilight
            {
                textEdit->setTextColor(QColor(255, 0, 0, 255)); // red
                bHilight = true;

                mutex_notify.lock();
                notify->play();
                mutex_notify.unlock();

                bLevel = true;
            }
        }
        else if ((i > 10) && (bLevel == false))
        {
            if (iLevel == 0)
                textEdit->setTextColor(QColor(0, 0, 0, 255)); // black
            else if (iLevel == 1) // join
                textEdit->setTextColor(QColor(0, 147, 0, 255)); // green
            else if (iLevel == 2) // part
                textEdit->setTextColor(QColor(71, 51, 255, 255)); // light blue
            else if (iLevel == 3) // quit
                textEdit->setTextColor(QColor(0, 0, 127, 255)); // dark blue
            else if (iLevel == 4) // kick
                textEdit->setTextColor(QColor(0, 0, 127, 255)); // dark blue
            else if (iLevel == 5) // mode
                textEdit->setTextColor(QColor(0, 147, 0, 255)); // green
            else if (iLevel == 6) // notice
                textEdit->setTextColor(QColor(0, 102, 255, 255)); // blue
            else if (iLevel == 7) // info
                textEdit->setTextColor(QColor(102, 102, 102, 255)); // gray
            else if (iLevel == 9) // error
                textEdit->setTextColor(QColor(255, 0, 0, 255)); // red

            bLevel = true;
        }

        // /me
        if (QString(strData[i]) == QString(QByteArray("\x01")))
        {
            if (settings->value("hide_formating").toString() == "off")
                textEdit->setAlignment(Qt::AlignHCenter);
        }
        // emoticons
        else if ((strData[i] == '%') && (strData.length() > i+1) && (strData[i+1] == 'I'))
        {
            int x = strData.indexOf("%",i+1);
            int y = strData.indexOf(" ",i+1);
            if ((x != -1) && ((x < y) || (y == -1)))
            {
                QString path = QCoreApplication::applicationDirPath();
                QString emoticon = strData.mid(i+2,x-i-2).toLower();
                QString emoticonFull1 = path+"/3rdparty/emoticons/"+emoticon+".gif";
                QString emoticonFull2 = path+"/3rdparty/emoticons_other/"+emoticon+".gif";
                QFile f1(emoticonFull1);
                QFile f2(emoticonFull2);
                if ((f1.exists() == true) && (settings->value("hide_formating").toString() == "off"))
                {
                    QColor realcolor = textEdit->textColor(); // fix font color
                    QFont realfont = textEdit->currentFont(); // fix font size
                    textEdit->textCursor().insertImage(QImage(emoticonFull1));
                    textEdit->setCurrentFont(realfont); // fix font size
                    textEdit->setTextColor(realcolor); // fix font color
                }
                else if ((f2.exists() == true) && (settings->value("hide_formating").toString() == "off"))
                {
                    QColor realcolor = textEdit->textColor(); // fix font color
                    QFont realfont = textEdit->currentFont(); // fix font size
                    textEdit->textCursor().insertImage(QImage(emoticonFull2));
                    textEdit->setCurrentFont(realfont); // fix font size
                    textEdit->setTextColor(realcolor); // fix font color
                }
                // emoticon not exist or hide formating
                else
                    textEdit->insertPlainText("//"+emoticon);
                i = x;
            }
            // not emoticon
            else
                textEdit->insertPlainText(QString(strData[i]));
        }
        // font color
        else if ((strData[i] == '%') && (strData.length() > i+1) && (strData[i+1] == 'C'))
        {
            int x = strData.indexOf("%",i+1);
            int y = strData.indexOf(" ",i+1);
            if ((x != -1) && ((x < y) || (y == -1)))
            {
                QString strColor = strData.mid(i+2,x-i-2).toLower();
                if ((strColor == "000000") || (strColor == "623c00") || (strColor == "c86c00") || (strColor == "ff6500") || (strColor == "ff0000") || (strColor == "e40f0f") || (strColor == "990033") || (strColor == "8800ab") || (strColor == "ce00ff") || (strColor == "0f2ab1") || (strColor == "3030ce") || (strColor == "006699") || (strColor == "1a866e") || (strColor == "008100") || (strColor == "959595"))
                {
                    bool ok;
                    int r = strColor.mid(0,2).toUInt(&ok,16);
                    int g = strColor.mid(2,2).toUInt(&ok,16);
                    int b = strColor.mid(4,2).toUInt(&ok,16);

                    if ((bHilight == false) && (settings->value("hide_formating").toString() == "off"))
                        textEdit->setTextColor(QColor(r, g, b, 255));

                    i = x;
                }
                // color not supported or hide formating
                else
                    textEdit->insertPlainText(QString(strData[i]));
            }
            // not color
            else
                textEdit->insertPlainText(QString(strData[i]));
        }
        // font
        else if ((strData[i] == '%') && (strData.length() > i+1) && (strData[i+1] == 'F'))
        {
            int x = strData.indexOf("%",i+1);
            int y = strData.indexOf(" ",i+1);
            if ((x != -1) && ((x < y) || (y == -1)))
            {
                QString strFont = strData.mid(i+2,x-i-2).toLower();

                if (settings->value("hide_formating").toString() == "off")
                {
                    if (strFont.indexOf(":") != -1)
                    {
                        QString strFontWeight = strFont.left(strFont.indexOf(":"));
                        QString strFontName = strFont.right(strFont.length()-strFont.indexOf(":")-1);

                         for (int fw = 0; fw < strFontWeight.length(); fw++)
                         {
                             if (strFontWeight[fw] == 'b') textEdit->setFontWeight(75);
                             else if (strFontWeight[fw] == 'i') textEdit->setFontItalic(true);
                         }

                         if (strFontName == "arial") textEdit->setFontFamily("Arial");
                         else if (strFontName == "times") textEdit->setFontFamily("Times New Roman");
                         else if (strFontName == "verdana") textEdit->setFontFamily("Verdana");
                         else if (strFontName == "tahoma") textEdit->setFontFamily("Tahoma");
                         else if (strFontName == "courier") textEdit->setFontFamily("Courier New");
                    }
                    else
                    {
                         if (strFont == "arial") textEdit->setFontFamily("Arial");
                         else if (strFont == "times") textEdit->setFontFamily("Times New Roman");
                         else if (strFont == "verdana") textEdit->setFontFamily("Verdana");
                         else if (strFont == "tahoma") textEdit->setFontFamily("Tahoma");
                         else if (strFont == "courier") textEdit->setFontFamily("Courier New");
                         else
                         {
                             for (int fw = 0; fw < strFont.length(); fw++)
                             {
                                 if (strFont[fw] == 'b') textEdit->setFontWeight(75);
                                 else if (strFont[fw] == 'i') textEdit->setFontItalic(true);
                             }
                         }
                    }
                }
                i = x;
            }
            else
                textEdit->insertPlainText(QString(strData[i]));
        }
        // link
        else if (
                ((strData[i] == 'h') && (strData.length() > i+1) && (strData[i+1] == 't') && (strData.length() > i+2) && (strData[i+2] == 't') && (strData.length() > i+3) && (strData[i+3] == 'p') && (strData.length() > i+4) && (strData[i+4] == 's') && (strData.length() > i+5) && (strData[i+5] == ':')) ||
                ((strData[i] == 'h') && (strData.length() > i+1) && (strData[i+1] == 't') && (strData.length() > i+2) && (strData[i+2] == 't') && (strData.length() > i+3) && (strData[i+3] == 'p') && (strData.length() > i+4) && (strData[i+4] == ':'))
            )
        {
            int x = strData.indexOf(" ",i+1);
            int y = strData.indexOf("href");

            if (x == -1)
                x = strData.length()-1;

            if ((y != i-6) && (y != i-5))
            {
                QString strLink = strData.mid(i,x-i).toLower();

                if (settings->value("hide_formating").toString() == "off")
                {
                    QColor realcolor = textEdit->textColor(); // fix font color
                    QFont realfont = textEdit->currentFont(); // fix font size
                    textEdit->insertHtml(QString("<a href=\"%1\">%1</a> ").arg(strLink));
                    textEdit->setCurrentFont(realfont); // fix font size
                    textEdit->setTextColor(realcolor); // fix font color
                }
                else
                    textEdit->insertPlainText(QString("%1 ").arg(strLink));

                i = x;
                if (i == strData.length()-1) // fix position
                    i--;
            }
            else
                textEdit->insertPlainText(QString(strData[i]));
        }
        // not action, emoticon, color, font, link
        else
            textEdit->insertPlainText(QString(strData[i]));
    }

    textEdit->insertHtml("</p>");
    textEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

// window options

void tab_widget::set_topic(QString strTopic)
{
    topic->setText(strTopic);
    topic->setCursorPosition(0);

    strTopic.replace(QRegExp("%C(\\S+)%"),"");
    strTopic.replace(QRegExp("%F(\\S+)%"),"");
    strTopic.replace(QRegExp("%I(\\S+)%"),"<\\1>");
    topic->setToolTip(strTopic);
}

void tab_widget::enable_topic()
{
    topic->setReadOnly(false);
    topicButton->show();
}

void tab_widget::disable_topic()
{
    topic->setReadOnly(true);
    topicButton->hide();
}

void tab_widget::enable_moderation()
{
    moderation->show();
    moderSendButton->show();
}

void tab_widget::disable_moderation()
{
    moderation->hide();
    moderSendButton->hide();
}

void tab_widget::author_topic(QString strAuthor)
{
    topicDetails->setText(QString(" Autor tematu: %1").arg(strAuthor));
}

void tab_widget::set_link(QString strUrl)
{
    webLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >Strona kana³u</a>").arg(strUrl));
    webLink->setToolTip(strUrl);
}

// nick list

void tab_widget::add_user(QString strNick, QString strSuffix)
{
    QString strmFlag;
    QString strmSuffix;
    bool bOwner = false;      // `
    bool bOp = false;         // @
    bool bHalfop = false;     // %
    bool bMod = false;        // !
    bool bVip = false;        // +
    bool bScreener = false;   // =

    bool bBusy = false;       // b
    bool bRestricted = false; // r
    bool bEncrypted = false;  // x
    bool bPublicCam = false;  // W
    bool bPrivCam = false;    // V
    bool bAdmin = false;      // o

    for (int i = 0; i < strNick.length(); i++)
    {
        switch(strNick[i].toAscii())
        {
            case '`':
                    bOwner = true;
                    strNick = strNick.right(strNick.length()-1);
                    strmFlag.append("`");
                    i--;
                    break;
            case '@':
                    bOp = true;
                    strNick = strNick.right(strNick.length()-1);
                    strmFlag.append("@");
                    i--;
                    break;
            case '%':
                    bHalfop = true;
                    strNick = strNick.right(strNick.length()-1);
                    strmFlag.append("%");
                    i--;
                    break;
            case '!':
                    bMod = true;
                    strNick = strNick.right(strNick.length()-1);
                    strmFlag.append("!");
                    i--;
                    break;
            case '+':
                    bVip = true;
                    strNick = strNick.right(strNick.length()-1);
                    strmFlag.append("+");
                    i--;
                    break;
            case '=':
                    bScreener = true;
                    strNick = strNick.right(strNick.length()-1);
                    strmFlag.append("=");
                    i--;
                    break;
            default:
                    break;
        }
    }

    for (int i = 0; i < strSuffix.length(); i++)
    {
        switch(strSuffix[i].toAscii())
        {
            case 'b':
                    bBusy = true;
                    strmSuffix.append("b");
                    break;
            case 'r':
                    bRestricted = true;
                    strmSuffix.append("r");
                    break;
            case 'x':
                    bEncrypted = true;
                    strmSuffix.append("x");
                    break;
            case 'W':
                    bPublicCam = true;
                    strmSuffix.append("W");
                    break;
            case 'V':
                    bPrivCam = true;
                    strmSuffix.append("V");
                    break;
            case 'o':
                    bAdmin = true;
                    strmSuffix.append("o");
                    break;
            default:
                    break;
        }
    }

    nick_flag.insert(strNick, strmFlag);
    nick_suffix.insert(strNick, strmSuffix);
    QString strStatus;

    if (bPublicCam == true)
    {
        if (bAdmin == true) strStatus = "admincam";
        else if (bOwner == true) strStatus = "ownercam";
        else if (bOp == true) strStatus = "opcam";
        else if (bHalfop == true) strStatus = "halfopcam";
        else if (bMod == true) strStatus = "modcam";
        else if (bVip == true) strStatus = "vipcam";
        else strStatus = "usercam";
    }
    else
    {
        if (bAdmin == true) strStatus = "admin";
        else if (bOwner == true) strStatus = "owner";
        else if (bOp == true) strStatus = "op";
        else if (bHalfop == true) strStatus = "halfop";
        else if (bMod == true) strStatus = "mod";
        else if (bVip == true) strStatus = "vip";
        else strStatus = "user";
    }

    if (tab_widget::nicklist_exist(strNick) == false)
    {
        tab_widget::nicklist_add(strNick, strStatus);

        inputline->set_userslist(nick_list);

        iNickCount++;
        nickCount->setText(QString("Liczba osób: %1").arg(iNickCount));

        if (iNickCount > 300)
            timer->setInterval(1000*60*60*1); // 1h
        else
            timer->setInterval(1000*60*60*4); // 4h
    }
}

void tab_widget::del_user(QString strNick)
{
    if (tab_widget::nicklist_exist(strNick) == true)
    {
        nick_flag.remove(strNick);
        nick_suffix.remove(strNick);

        tab_widget::nicklist_remove(strNick);

        inputline->set_userslist(nick_list);

        iNickCount--;
        nickCount->setText(QString("Liczba osób: %1").arg(iNickCount));

        if (iNickCount > 300)
            timer->setInterval(1000*60*60*1); // 1h
        else
            timer->setInterval(1000*60*60*4); // 4h
    }
}

void tab_widget::nicklist_add(QString strNick, QString strStatus)
{
    nicklist.insert(strNick, strStatus);
    tab_widget::nicklist_refresh();
}

void tab_widget::nicklist_remove(QString strNick)
{
    nicklist.remove(strNick);
    tab_widget::nicklist_refresh();
}

bool tab_widget::nicklist_exist(QString strNick)
{
    return nicklist.contains(strNick);
}

void tab_widget::nicklist_clear()
{
    nicklist.clear();
    tab_widget::nicklist_refresh();
}

void tab_widget::nicklist_refresh()
{
    mutex_nicklist.lock();
    nick_list->clear();

    new_nicklist1.clear();
    new_nicklist2.clear();

    tab_widget::nicklist_sort();

    while (!new_nicklist1.isEmpty())
    {
        QString strNick = new_nicklist1.dequeue();
        QString strStatus = new_nicklist2.dequeue();
        QIcon icon;
        if (strStatus == "admincam") icon = QIcon(":/3rdparty/images/admincam.png");
        else if (strStatus == "ownercam") icon = QIcon(":/3rdparty/images/ownercam.png");
        else if (strStatus == "opcam") icon = QIcon(":/3rdparty/images/opcam.png");
        else if (strStatus == "halfopcam") icon = QIcon(":/3rdparty/images/halfopcam.png");
        else if (strStatus == "modcam") icon = QIcon(":/3rdparty/images/modcam.png");
        else if (strStatus == "vipcam") icon = QIcon(":/3rdparty/images/vipcam.png");
        else if (strStatus == "usercam") icon = QIcon(":/3rdparty/images/usercam.png");
        else if (strStatus == "admin") icon = QIcon(":/3rdparty/images/admin.png");
        else if (strStatus == "owner") icon = QIcon(":/3rdparty/images/owner.png");
        else if (strStatus == "op") icon = QIcon(":/3rdparty/images/op.png");
        else if (strStatus == "halfop") icon = QIcon(":/3rdparty/images/halfop.png");
        else if (strStatus == "mod") icon = QIcon(":/3rdparty/images/mod.png");
        else if (strStatus == "vip") icon = QIcon(":/3rdparty/images/vip.png");
        else if (strStatus == "user") icon = QIcon(":/3rdparty/images/user.png");

        QListWidgetItem *new_nick;
        new_nick = new QListWidgetItem(icon, strNick);
        nick_list->addItem(new_nick);
    }

    mutex_nicklist.unlock();
}

void tab_widget::nicklist_sort()
{
    tab_widget::nicklist_quicksort("admin");
    tab_widget::nicklist_quicksort("owner");
    tab_widget::nicklist_quicksort("op");
    tab_widget::nicklist_quicksort("halfop");
    tab_widget::nicklist_quicksort("mod");
    tab_widget::nicklist_quicksort("vip");
    tab_widget::nicklist_quicksort("user");
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

void tab_widget::nicklist_quicksort(QString strStatus)
{
    QHash <QString, QString> status_nicklist;

    QHash <QString, QString>::const_iterator i1 = nicklist.constBegin();
    while (i1 != nicklist.constEnd())
    {
        if ((i1.value() == strStatus) || (i1.value() == strStatus+"cam"))
            status_nicklist.insert(i1.key(), i1.value());

        ++i1;
    }

    QStringList newlist;

    QHash <QString, QString>::const_iterator i2 = status_nicklist.constBegin();
    while (i2 != status_nicklist.constEnd())
    {
        newlist.insert(newlist.count(), i2.key());
        ++i2;
    }

    qStableSort(newlist.begin(), newlist.end(), caseInsensitiveLessThan);

    for (int i = 0; i < newlist.count(); i++)
    {
        QString strNick = newlist.at(i);
        new_nicklist1.insert(new_nicklist1.count(), strNick);
        new_nicklist2.insert(new_nicklist2.count(), status_nicklist[strNick]);
    }
}

void tab_widget::change_flag(QString strNick, QString strNewFlag)
{
    QString strFlag = nick_flag[strNick];
    QString strSuffix = nick_suffix[strNick];

    if ((strNewFlag == "+q") && (strFlag.indexOf("`") == -1)) strFlag.append("`");
    else if ((strNewFlag == "-q") && (strFlag.indexOf("`") != -1)) strFlag.remove("`");
    else if ((strNewFlag == "+o") && (strFlag.indexOf("@") == -1)) strFlag.append("@");
    else if ((strNewFlag == "-o") && (strFlag.indexOf("@") != -1)) strFlag.remove("@");
    else if ((strNewFlag == "+h") && (strFlag.indexOf("%") == -1)) strFlag.append("%");
    else if ((strNewFlag == "-h") && (strFlag.indexOf("%") != -1)) strFlag.remove("%");
    else if ((strNewFlag == "+v") && (strFlag.indexOf("+") == -1)) strFlag.append("+");
    else if ((strNewFlag == "-v") && (strFlag.indexOf("+") != -1)) strFlag.remove("+");
    else if ((strNewFlag == "+X") && (strFlag.indexOf("!") == -1)) strFlag.append("!");
    else if ((strNewFlag == "-X") && (strFlag.indexOf("!") != -1)) strFlag.remove("!");
    else if ((strNewFlag == "+Y") && (strFlag.indexOf("=") == -1)) strFlag.append("=");
    else if ((strNewFlag == "-Y") && (strFlag.indexOf("=") != -1)) strFlag.remove("=");
    else if ((strNewFlag == "+b") && (strSuffix.indexOf("b") == -1)) strSuffix.append("b");
    else if ((strNewFlag == "-b") && (strSuffix.indexOf("b") != -1)) strSuffix.remove("b");
    else if ((strNewFlag == "+r") && (strSuffix.indexOf("r") == -1)) strSuffix.append("r");
    else if ((strNewFlag == "-r") && (strSuffix.indexOf("r") != -1)) strSuffix.remove("r");
    else if ((strNewFlag == "+W") && (strSuffix.indexOf("W") == -1)) strSuffix.append("W");
    else if ((strNewFlag == "-W") && (strSuffix.indexOf("W") != -1)) strSuffix.remove("W");
    else if ((strNewFlag == "+V") && (strSuffix.indexOf("V") == -1)) strSuffix.append("V");
    else if ((strNewFlag == "-V") && (strSuffix.indexOf("V") != -1)) strSuffix.remove("V");
    else if ((strNewFlag == "+x") && (strSuffix.indexOf("x") == -1)) strSuffix.append("x");
    else if ((strNewFlag == "-x") && (strSuffix.indexOf("x") != -1)) strSuffix.remove("x");

    tab_widget::del_user(strNick);
    tab_widget::add_user(strFlag+strNick, strSuffix);

    config *pConfig = new config();
    QString strMe = pConfig->get_value("login-nick");
    delete pConfig;

    if (strNick == strMe)
    {
        if (strNewFlag == "+q") tab_widget::enable_topic();
        else if (strNewFlag == "-q") tab_widget::disable_topic();
        else if (strNewFlag == "+o") tab_widget::enable_topic();
        else if (strNewFlag == "-o") tab_widget::disable_topic();
        else if (strNewFlag == "+h") tab_widget::enable_topic();
        else if (strNewFlag == "-h") tab_widget::disable_topic();
        else if (strNewFlag == "+X") tab_widget::enable_moderation();
        else if (strNewFlag == "-X") tab_widget::disable_moderation();
    }
}

void tab_widget::update_nick(QString strNick)
{
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(strNick));
}

void tab_widget::clear_nicklist()
{
    iNickCount = 0;
    nickCount->setText(QString("Liczba osób: %1").arg(iNickCount));
    nick_flag.clear();
    nick_suffix.clear();
    nick_list->clear();
    nicklist.clear();
}

void tab_widget::set_open_channels(QStringList strOpenChannels)
{
    nick_list->set_open_channels(strOpenChannels);
}

// copy of network::send
void tab_widget::send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "-> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        socket->write(qbaData);
        if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->waitForBytesWritten() == false))
           tab_widget::display_msg(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
    }
    else
        tab_widget::display_msg("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

// actions

void tab_widget::bold_clicked()
{
    if (bBold == true)
    {
        bold->setDown(false);
        bBold = false;
        inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
    }
    else
    {
        bold->setDown(true);
        bBold = true;
        inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
    }
}

void tab_widget::italic_clicked()
{
    if (bItalic == true)
    {
        italic->setDown(false);
        bItalic = false;

        if ((bBold == true) && (bItalic == true))
            inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
        else if ((bBold == true) && (bItalic == false))
            inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
        else if ((bBold == false) && (bItalic == true))
            inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
        else if ((bBold == false) && (bItalic == false))
            inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
    }
    else
    {
        italic->setDown(true);
        bItalic = true;

        if ((bBold == true) && (bItalic == true))
            inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
        else if ((bBold == true) && (bItalic == false))
            inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
        else if ((bBold == false) && (bItalic == true))
            inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
        else if ((bBold == false) && (bItalic == false))
            inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
    }
}

void tab_widget::arial_triggered()
{
    fontfamily->setText("Arial");

    if ((bBold == true) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
    else if ((bBold == true) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
    else if ((bBold == false) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
    else if ((bBold == false) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
}

void tab_widget::times_triggered()
{
    fontfamily->setText("Times");

    if ((bBold == true) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
    else if ((bBold == true) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
    else if ((bBold == false) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
    else if ((bBold == false) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
}

void tab_widget::verdana_triggered()
{
    fontfamily->setText("Verdana");

    if ((bBold == true) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
    else if ((bBold == true) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
    else if ((bBold == false) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
    else if ((bBold == false) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
}

void tab_widget::tahoma_triggered()
{
    fontfamily->setText("Tahoma");

    if ((bBold == true) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
    else if ((bBold == true) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
    else if ((bBold == false) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
    else if ((bBold == false) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
}

void tab_widget::courier_triggered()
{
    fontfamily->setText("Courier");

    if ((bBold == true) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, true));
    else if ((bBold == true) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 75, false));
    else if ((bBold == false) && (bItalic == true))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, true));
    else if ((bBold == false) && (bItalic == false))
        inputline->setFont(QFont(fontfamily->text(), -1, 50, false));
}

void tab_widget::size8_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(8);
}

void tab_widget::size9_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(9);
}

void tab_widget::size10_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(10);
}

void tab_widget::size11_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(11);
}

void tab_widget::size12_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(12);
}

void tab_widget::size14_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(14);
}

void tab_widget::size18_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(18);
}

void tab_widget::size20_triggered()
{
    textEdit->selectAll();
    textEdit->setFontPointSize(20);
}

// color

void tab_widget::color_clicked(int index)
{
    if (index == 0) strCurrentColor = "000000";
    else if (index == 1) strCurrentColor = "623c00";
    else if (index == 2) strCurrentColor = "c86c00";
    else if (index == 3) strCurrentColor = "ff6500";
    else if (index == 4) strCurrentColor = "ff0000";
    else if (index == 5) strCurrentColor = "e40f0f";
    else if (index == 6) strCurrentColor = "990033";
    else if (index == 7) strCurrentColor = "8800ab";
    else if (index == 8) strCurrentColor = "ce00ff";
    else if (index == 9) strCurrentColor = "0f2ab1";
    else if (index == 10) strCurrentColor = "3030ce";
    else if (index == 11) strCurrentColor = "006699";
    else if (index == 12) strCurrentColor = "1a866e";
    else if (index == 13) strCurrentColor = "008100";
    else if (index == 14) strCurrentColor = "959595";
    else strCurrentColor = "000000";
    inputline->setStyleSheet(QString("color: #"+strCurrentColor));
}

// input line

void tab_widget::inputline_return_pressed()
{
    QString strTextO = inputline->text();
    QStringList strTextA = strTextO.split(QRegExp("(\n|\r)"));

    for (int i = 0; i < strTextA.count(); i++)
    {
        QString strText = strTextA[i];
        QString strTextOriginal = strText;
        strLast_msg = strText;

        config *pConfig = new config();
        QString strMe = pConfig->get_value("login-nick");
        delete pConfig;

        if ((strText[0] == '/') && (strText[1] != '/'))
        {
            if (strText[0] == '/')
                strText = strText.right(strText.length()-1);
            strTextOriginal = strText;
            QStringList strTextList = strText.split(" ");

            commands *pCommands = new commands(strName, strText, settings);
            strText = pCommands->execute();
            delete pCommands;

            if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
                tab_widget::display_msg(strText, 7);
            else if (strTextList[0] == "me")
            {
                if (strTextOriginal.length() > 3)
                {
                    QString strTextSend = strText;
                    QString strTextDisplay = strTextOriginal.right(strTextOriginal.length()-3);

                    QString weight;
                    QString font = fontfamily->text().toLower();

                    if (bBold == true) weight += "b";
                    if (bItalic == true) weight += "i";

                    if ((weight != "") || (font != "verdana"))
                        strTextDisplay = "%F"+weight+":"+font+"%"+strTextDisplay;
                    if (strCurrentColor != "000000")
                        strTextDisplay = "%C"+strCurrentColor+"%"+strTextDisplay;

                    QDateTime dt = QDateTime::currentDateTime();
                    QString strDT = dt.toString("[hh:mm:ss] ");

                    log *l = new log();
                    l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strTextDisplay));

                    tab_widget::display_message(QString("%1<%2> %3ACTION %4%5").arg(strDT).arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01"))), 0);
                    if (socket->state() == QAbstractSocket::ConnectedState)
                        tab_widget::send(strTextSend);
                }
            }
            else
            {
                if ((socket->state() == QAbstractSocket::ConnectedState) && (strText.length() > 0))
                    tab_widget::send(strText);
            }

            inputline->clear();
        }
        else if (strName != "Status")
        {
            if ((socket->state() == QAbstractSocket::ConnectedState) && (strText.length() > 0))
            {
                QString weight;
                QString font = fontfamily->text().toLower();

                if (bBold == true) weight += "b";
                if (bItalic == true) weight += "i";

                if ((weight != "") || (font != "verdana"))
                    strText = "%F"+weight+":"+font+"%"+strText;
                if (strCurrentColor != "000000")
                    strText = "%C"+strCurrentColor+"%"+strText;

                strText = tab_widget::convert_emots(strText);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                log *l = new log();
                l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText));

                strText = QString("PRIVMSG %1 :%2").arg(strName).arg(strText);
                tab_widget::send(strText);
                tab_widget::display_message(QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-10-strName.length())), 0);

                inputline->clear();
            }
        }
    }
}

void tab_widget::moder_button_clicked()
{
    QString strTextO = inputline->text();
    QStringList strTextA = strTextO.split(QRegExp("(\n|\r)"));

    for (int i = 0; i < strTextA.count(); i++)
    {
        QString strText = strTextA[i];
        QString strTextOriginal = strText;
        strLast_msg = strText;

        config *pConfig = new config();
        QString strMe = pConfig->get_value("login-nick");
        delete pConfig;

        if ((strText[0] == '/') && (strText[1] != '/'))
        {
            if (strText[0] == '/')
                strText = strText.right(strText.length()-1);
            strTextOriginal = strText;
            QStringList strTextList = strText.split(" ");

            commands *pCommands = new commands(strName, strText, settings);
            strText = pCommands->execute();
            delete pCommands;

            if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
                tab_widget::display_msg(strText, 7);
            else if (strTextList[0] == "me")
            {
                if (strTextOriginal.length() > 3)
                {
                    QString strTextSend = strText;
                    QString strTextDisplay = strTextOriginal.right(strTextOriginal.length()-3);

                    QString weight;
                    QString font = fontfamily->text().toLower();

                    if (bBold == true) weight += "b";
                    if (bItalic == true) weight += "i";

                    if ((weight != "") || (font != "verdana"))
                        strTextDisplay = "%F"+weight+":"+font+"%"+strTextDisplay;
                    if (strCurrentColor != "000000")
                        strTextDisplay = "%C"+strCurrentColor+"%"+strTextDisplay;

                    QDateTime dt = QDateTime::currentDateTime();
                    QString strDT = dt.toString("[hh:mm:ss] ");

                    log *l = new log();
                    l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strTextDisplay));

                    tab_widget::display_message(QString("%1<%2> %3ACTION %4%5").arg(strDT).arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01"))), 0);
                    if (socket->state() == QAbstractSocket::ConnectedState)
                        tab_widget::send(strTextSend);
                }
            }
            else
            {
                if ((socket->state() == QAbstractSocket::ConnectedState) && (strText.length() > 0))
                    tab_widget::send(strText);
            }

            inputline->clear();
        }
        else if (strName != "Status")
        {
            if ((socket->state() == QAbstractSocket::ConnectedState) && (strText.length() > 0))
            {
                QString weight;
                QString font = fontfamily->text().toLower();

                if (bBold == true) weight += "b";
                if (bItalic == true) weight += "i";

                if ((weight != "") || (font != "verdana"))
                    strText = "%F"+weight+":"+font+"%"+strText;
                if (strCurrentColor != "000000")
                    strText = "%C"+strCurrentColor+"%"+strText;

                strText = tab_widget::convert_emots(strText);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                log *l = new log();
                l->save(strName, QString("%1* <%2> %3").arg(strDT).arg(strMe).arg(strText));

                strText = QString("MODERNOTICE %1 :%2").arg(strName).arg(strText);
                tab_widget::send(strText);
                tab_widget::display_message(QString("%1 *<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-14-strName.length())), 6);

                inputline->clear();
            }
        }
    }
}

void tab_widget::channel_settings_clicked()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        if (strName != "Status")
        {
            dlgchannel_settings->set_channel(strName);
            dlgchannel_settings->show();
        }
    }
}

void tab_widget::moderation_clicked()
{
    dlgmoderation->show();
}

void tab_widget::topic_return_pressed()
{
    QString strText = topic->text();
    tab_widget::send(QString("CS SET %1 TOPIC %2").arg(strName).arg(strText));
}

// fix if somebody move cursor
void tab_widget::cursor_position_changed()
{
    bCursorPositionChanged = true;
}

void tab_widget::timer_timeout()
{
    this->textEdit->clear();
}

void tab_widget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up)
    {
        inputline->clear();
        inputline->setText(strLast_msg);
        inputline->setFocus();
    }
    else if (e->key() == Qt::Key_Down)
    {
        inputline->clear();
        inputline->setFocus();
    }

    QWidget::keyPressEvent(e);
}
