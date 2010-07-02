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

TabWidget::TabWidget(Network *param1, QSettings *param2, QString param3, QWidget *parent, Notify *param4, QMap <QString, QByteArray> *param5, QMap <QString, QByteArray> *param6, DlgChannelSettings *param7, DlgModeration *param8)
{
    pNetwork = param1;
    settings = param2;
    strName = param3;
    pNotify = param4;
    mNickAvatar = param5;
    mChannelAvatar = param6;
    dlgchannel_settings = param7;
    dlgmoderation = param8;

    iNickCount = 0;
    bCursorPositionChanged = false;
    strCurrentColor = "000000";
    strFontSize = "11px";
    strContentStart = "<html><body style=\"background-color:#ffffff;\">";
    strContentEnd = "</body></html>";

    splitter = new QSplitter(this);
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    leftWidget = new QWidget(this);
    rightWidget = new QWidget(this);

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

    logo = new QLabel(this);
    logo->setParent(this);
    logo->show();

    topLeftUpWidget = new QWidget(this);
    topLeftUpLayout = new QHBoxLayout();
    topLeftUpLayout->setMargin(0);
    topLeftUpLayout->setAlignment(Qt::AlignLeft);
    topLeftUpLayout->addWidget(topic);
    topLeftUpLayout->addWidget(topicButton);
    topLeftUpWidget->setLayout(topLeftUpLayout);

    topLeftWidget = new QWidget(this);
    topLeftLayout = new QVBoxLayout();
    topLeftLayout->setMargin(0);
    topLeftLayout->setAlignment(Qt::AlignTop);
    topLeftLayout->addWidget(topLeftUpWidget);
    topLeftLayout->addWidget(topicDetails);
    topLeftWidget->setLayout(topLeftLayout);

    topRightWidget = new QWidget(this);
    topRightLayout = new QVBoxLayout();
    topRightLayout->setMargin(0);
    topRightLayout->setAlignment(Qt::AlignTop);
    topRightLayout->addWidget(logo);
    topRightWidget->setLayout(topRightLayout);

    topWidget = new QWidget(this);
    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->setAlignment(Qt::AlignLeft);
    topLayout->addWidget(topLeftWidget);
    topLayout->addWidget(topRightWidget);
    topWidget->setLayout(topLayout);

    webLink = new QLabel();
    webLink->setParent(this);
    webLink->setOpenExternalLinks(true);
    webLink->setAlignment(Qt::AlignCenter);
    webLink->show();

    nickCount = new QLabel();
    nickCount->setParent(this);
    nickCount->setOpenExternalLinks(true);
    nickCount->setAlignment(Qt::AlignCenter);
    nickCount->show();

    nick_list = new Nicklist(pNetwork, settings, strName, mNickAvatar);
    nick_list->setParent(this);
    nick_list->setSortingEnabled(false);
    nick_list->setItemDelegate(new NicklistDelegate(nick_list));
    nick_list->show();

    textEdit = new QWebView(this);
    textEdit->setParent(this);
    textEdit->show();

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
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
    size->setParent(this);
    size->setMaximumWidth(250);
    size->setMaximumHeight(25);
    size->setMenu(sizeMenu);
    size->show();

    color = new QComboBox(this);
    color->setParent(this);
    color->insertItem(0, QIcon(":/images/000000.png"),"");
    color->insertItem(1, QIcon(":/images/623c00.png"),"");
    color->insertItem(2, QIcon(":/images/c86c00.png"),"");
    color->insertItem(3, QIcon(":/images/ff6500.png"),"");
    color->insertItem(4, QIcon(":/images/ff0000.png"),"");
    color->insertItem(5, QIcon(":/images/e40f0f.png"),"");
    color->insertItem(6, QIcon(":/images/990033.png"),"");
    color->insertItem(7, QIcon(":/images/8800ab.png"),"");
    color->insertItem(8, QIcon(":/images/ce00ff.png"),"");
    color->insertItem(9, QIcon(":/images/0f2ab1.png"),"");
    color->insertItem(10, QIcon(":/images/3030ce.png"),"");
    color->insertItem(11, QIcon(":/images/006699.png"),"");
    color->insertItem(12, QIcon(":/images/1a866e.png"),"");
    color->insertItem(13, QIcon(":/images/008100.png"),"");
    color->insertItem(14, QIcon(":/images/959595.png"),"");
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

    toolWidget = new QWidget(this);
    toolLayout = new QHBoxLayout();
    toolLayout->setMargin(0);
    toolLayout->setAlignment(Qt::AlignLeft);
    toolLayout->addWidget(bold);
    toolLayout->addWidget(italic);
    toolLayout->addWidget(fontfamily);
    toolLayout->addWidget(color);
    toolLayout->addWidget(size);
    toolLayout->addWidget(channel_settings);
    toolLayout->addWidget(moderation);
    toolWidget->setLayout(toolLayout);

    nickLabel = new QLabel();
    nickLabel->setParent(this);
    nickLabel->show();

    inputline = new Inputline(this);
    inputline->setParent(this);
    inputline->setMinimumWidth(400);
    inputline->setMaxLength(300);
    inputline->setFont(QFont("Verdana", -1, -1, false));
    inputline->show();

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

    bottomWidget = new QWidget(this);
    bottomLayout = new QHBoxLayout();
    bottomLayout->setMargin(0);
    bottomLayout->setAlignment(Qt::AlignLeft);
    bottomLayout->addWidget(nickLabel);
    bottomLayout->addWidget(inputline);
    bottomLayout->addWidget(sendButton);
    bottomLayout->addWidget(moderSendButton);
    bottomWidget->setLayout(bottomLayout);

    mainLayout = new QGridLayout();

    if (strName[0] == '#')
    {
        topicButton->hide();
        topLeftUpLayout->removeWidget(topicButton);
        moderation->hide();
        toolLayout->removeWidget(moderation);
        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        rightLayout->addWidget(webLink);
        rightLayout->addWidget(nickCount);
        rightLayout->addWidget(nick_list);

        leftLayout->addWidget(topWidget);
        leftLayout->addWidget(textEdit);
        leftLayout->addWidget(toolWidget);
        leftLayout->addWidget(bottomWidget);

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
        logo->hide();
        nickCount->hide();
        webLink->hide();

        moderation->hide();
        toolLayout->removeWidget(moderation);
        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        rightLayout->addWidget(nick_list);

        leftLayout->addWidget(textEdit);
        leftLayout->addWidget(toolWidget);
        leftLayout->addWidget(bottomWidget);

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
        logo->hide();
        nickCount->hide();
        webLink->hide();
        nick_list->hide();

        moderation->hide();
        toolLayout->removeWidget(moderation);
        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        leftLayout->addWidget(textEdit);
        leftLayout->addWidget(toolWidget);
        leftLayout->addWidget(bottomWidget);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);
        mainLayout->addWidget(splitter);
    }

    if (strName == "Status") channel_settings->hide();
    this->setLayout(mainLayout);

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

    QObject::connect(textEdit, SIGNAL(loadFinished(bool)), this, SLOT(change_scroll_position()));
}

TabWidget::~TabWidget()
{
    nick_flag.clear();
    nick_suffix.clear();
    nickLabel->clear();
    strContent.clear();
    textEdit->setHtml(strContent, QUrl(""));
    nick_list->clear();
    nicklist.clear();

    delete nick_list;
    delete inputline;
}

QString TabWidget::convert_emots(QString strData)
{
    strData.replace(QRegExp("(http:|https:)//"), "\\1\\\\"); // fix http https
    strData.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
    strData.replace(QRegExp("(http:|https:)\\\\\\\\"), "\\1//"); // fix http https
    return strData;
}

QString TabWidget::replace_emots(QString strData)
{
    Replace *pReplace = new Replace();
    strData = pReplace->replace_emots(strData);
    delete pReplace;

    return strData;
}

void TabWidget::display_msg(QString strTime, QString strData, int iLevel)
{
    QDateTime dt = QDateTime::fromTime_t(strTime.toInt());
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    Log *l = new Log();
    l->save(strName, strData);

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings->value("hide_join_part").toString() == "on")
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_msg(QString strData, int iLevel)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    Log *l = new Log();
    l->save(strName, strData);

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings->value("hide_join_part").toString() == "on")
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_message(QString strData, int iLevel)
{
    if (strFontStyle != "normal") strFontStyle = "normal";
    if (strFontColor != "#000000") strFontColor = "#000000";
    if (strFontAlign != "left") strFontAlign = "left";
    if (strFontFamily != "Verdana") strFontFamily = "Verdana";
    if (strFontWeight != "normal") strFontWeight = "normal";
    if (strContent.count("</p>") > 250)
    {
        QStringList list = strContent.split("</p>");
        int iCount = strContent.count("</p>");
        strContent.clear();
        for (int i = iCount-250; i < iCount; i++)
            strContent.append(list.at(i)+"</p>");
        list.clear();
    }

    strData += " ";
    strContent.append("<p style=\"margin:0;padding:0;font-style:"+strFontStyle+";color:"+strFontColor+";text-align:"+strFontAlign+";font-family:"+strFontFamily+";font-weight:"+strFontWeight+";font-size:"+strFontSize+";\">");
    bool bLevel = false;
    QString strContentLast;

    // if /me remove time/action
    if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
    {
        strData = strData.right(strData.length() - 11);
        if (strData.indexOf("ACTION ") != -1) strData = strData.replace("ACTION ", QString::null);
        if (strData.indexOf("<") != -1) strData = strData.remove(strData.indexOf("<"),1);
        if (strData.indexOf(">") != -1) strData = strData.remove(strData.indexOf(">"),1);
    }

    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

    for (int i = 0; i < strData.length(); i++)
    {
        // colors
        if ((i > 10) && (bLevel == false))
        {
            if (iLevel == 0)
                strFontColor = "#000000"; // black
            else if (iLevel == 1) // join
                strFontColor = "#009300"; // green
            else if (iLevel == 2) // part
                strFontColor = "#4733FF"; // light blue
            else if (iLevel == 3) // quit
                strFontColor = "#00007F"; // dark blue
            else if (iLevel == 4) // kick
                strFontColor = "#00007F"; // dark blue
            else if (iLevel == 5) // mode
                strFontColor = "#009300"; // green
            else if (iLevel == 6) // notice
                strFontColor = "#0066FF"; // blue
            else if (iLevel == 7) // info
                strFontColor = "#666666"; // gray
            else if (iLevel == 9) // error
                strFontColor = "#ff0000"; // red

            strContent.append("<span style=\"color:"+strFontColor+";\">");
            strContentLast = "</span>"+strContentLast;
            bLevel = true;
        }

        // /me
        if (QString(strData[i]) == QString(QByteArray("\x01")))
        {
            if (settings->value("hide_formating").toString() == "off")
            {
                strFontAlign = "center";

                int iLast1 = strContent.lastIndexOf("text-align:");
                int iLast2 = strContent.indexOf(";",iLast1);
                strContent.remove(iLast1, iLast2-iLast1);
                strContent.insert(iLast1, "text-align:center");
            }
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
#ifdef Q_WS_X11
                    strContent.append("<img src=\"file://"+emoticonFull1+"\" alt=\""+emoticon+"\" />");
#else
                    strContent.append("<img src=\""+emoticonFull1+"\" alt=\""+emoticon+"\" />");
#endif
                }
                else if ((f2.exists() == true) && (settings->value("hide_formating").toString() == "off"))
                {
#ifdef Q_WS_X11
                    strContent.append("<img src=\"file://"+emoticonFull2+"\" alt=\""+emoticon+"\" />");
#else
                    strContent.append("<img src=\""+emoticonFull2+"\" alt=\""+emoticon+"\" />");
#endif
                }
                // emoticon not exist or hide formating
                else
                    strContent.append("//"+emoticon);
                i = x;
            }
            // not emoticon
            else
                strContent.append(QString(strData[i]));
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
                    if (settings->value("hide_formating").toString() == "off")
                    {
                        strFontColor = strColor;
                        strContent.append("<span style=\"color:"+strFontColor+";\">");
                        strContentLast = "</span>"+strContentLast;
                    }

                    i = x;
                }
                // color not supported or hide formating
                else
                    strContent.append(QString(strData[i]));
            }
            // not color
            else
                strContent.append(QString(strData[i]));
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
                             if (strFontWeight[fw] == 'b') strFontWeight = "bold";
                             else if (strFontWeight[fw] == 'i') strFontStyle = "italic";
                         }

                         if (strFontName == "arial") strFontFamily = "Arial";
                         else if (strFontName == "times") strFontFamily = "Times New Roman";
                         else if (strFontName == "verdana") strFontFamily = "Verdana";
                         else if (strFontName == "tahoma") strFontFamily = "Tahoma";
                         else if (strFontName == "courier") strFontFamily = "Courier New";

                         strContent.append("<span style=\"font-weight:"+strFontWeight+";font-style:"+strFontStyle+";font-family:"+strFontFamily+";\">");
                         strContentLast = "</span>"+strContentLast;
                    }
                    else
                    {
                         if (strFont == "arial") strFontFamily = "Arial";
                         else if (strFont == "times") strFontFamily = "Times New Roman";
                         else if (strFont == "verdana") strFontFamily = "Verdana";
                         else if (strFont == "tahoma") strFontFamily = "Tahoma";
                         else if (strFont == "courier") strFontFamily = "Courier New";
                         else
                         {
                             for (int fw = 0; fw < strFont.length(); fw++)
                             {
                                 if (strFont[fw] == 'b') strFontWeight = "bold";
                                 else if (strFont[fw] == 'i') strFontStyle = "italic";
                             }
                         }
                         strContent.append("<span style=\"font-weight:"+strFontWeight+";font-style:"+strFontStyle+";font-family:"+strFontFamily+";\">");
                         strContentLast = "</span>"+strContentLast;
                    }
                }
                i = x;
            }
            else
                strContent.append(QString(strData[i]));
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
                QString strLink = strData.mid(i,x-i);

                if (settings->value("hide_formating").toString() == "off")
                {
                    strContent.append("<span style=\"color:#0000ff;\">");
                    strContent.append(QString("%1 ").arg(strLink));
                    strContentLast = "</span>"+strContentLast;
                }
                else
                    strContent.append(QString("%1 ").arg(strLink));

                i = x;
                if (i == strData.length()-1) // fix position
                    i--;
            }
            else
                strContent.append(QString(strData[i]));
        }
        // not action, emoticon, color, font, link
        else
            strContent.append(QString(strData[i]));
    }

    // hilight
    if (iLevel == 8)
    {
        strContent.append("<hr>");

        pNotify->play();
    }

    strContent = strContent+strContentLast;
    strContent.append("</p>");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
}

// window options

void TabWidget::set_topic(QString strTopic)
{
    topic->setText(strTopic);
    topic->setCursorPosition(0);

    strTopic.replace(QRegExp("%C(\\S+)%"),"");
    strTopic.replace(QRegExp("%F(\\S+)%"),"");
    strTopic.replace(QRegExp("%I(\\S+)%"),"<\\1>");
    topic->setToolTip(strTopic);
}

void TabWidget::enable_topic()
{
    topic->setReadOnly(false);
    topicButton->show();
    topLeftUpLayout->addWidget(topicButton);
}

void TabWidget::disable_topic()
{
    topic->setReadOnly(true);
    topicButton->hide();
    topLeftUpLayout->removeWidget(topicButton);
}

void TabWidget::enable_moderation()
{
    moderation->show();
    toolLayout->addWidget(moderation);
    moderSendButton->show();
    bottomLayout->addWidget(moderSendButton);
}

void TabWidget::disable_moderation()
{
    moderation->hide();
    toolLayout->removeWidget(moderation);
    moderSendButton->hide();
    bottomLayout->removeWidget(moderSendButton);
}

void TabWidget::author_topic(QString strAuthor)
{
    topicDetails->setText(QString("Autor tematu: %1").arg(strAuthor));
}

void TabWidget::set_link(QString strUrl)
{
    webLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >Strona kana³u</a>").arg(strUrl));
    webLink->setToolTip(strUrl);
}

// nick list

void TabWidget::add_user(QString strNick, QString strSuffix, int iRefresh)
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

    if (nicklist_exist(strNick) == false)
    {
        nicklist_add(strNick, strStatus);
        if (iRefresh == 1)
            nicklist_refresh();

        inputline->set_userslist(nick_list);

        iNickCount++;
        nickCount->setText(QString("Liczba osób: %1").arg(iNickCount));
    }
}

void TabWidget::del_user(QString strNick)
{
    if (nicklist_exist(strNick) == true)
    {
        nick_flag.remove(strNick);
        nick_suffix.remove(strNick);

        nicklist_remove(strNick);

        inputline->set_userslist(nick_list);

        iNickCount--;
        nickCount->setText(QString("Liczba osób: %1").arg(iNickCount));
    }
}

void TabWidget::nicklist_add(QString strNick, QString strStatus)
{
    nick_list->nicklist_add(strNick, strStatus, &nicklist, &new_nicklist1, &new_nicklist2);
}

void TabWidget::nicklist_remove(QString strNick)
{
    nick_list->nicklist_remove(strNick, &nicklist, &new_nicklist1, &new_nicklist2);
}

bool TabWidget::nicklist_exist(QString strNick)
{
    return nick_list->nicklist_exist(strNick, &nicklist);
}

void TabWidget::nicklist_clear()
{
    nick_list->nicklist_clear(&nicklist, &new_nicklist1, &new_nicklist2);
}

void TabWidget::nicklist_refresh()
{
    nick_list->nicklist_refresh(&nicklist, &new_nicklist1, &new_nicklist2);
}

void TabWidget::nicklist_sort()
{
    nick_list->nicklist_sort(&nicklist, &new_nicklist1, &new_nicklist2);
}

void TabWidget::nicklist_quicksort(QString strStatus)
{
    nick_list->nicklist_quicksort(strStatus, &nicklist, &new_nicklist1, &new_nicklist2);
}

void TabWidget::change_flag(QString strNick, QString strNewFlag)
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

    del_user(strNick);
    add_user(strFlag+strNick, strSuffix, 1);

    Config *pConfig = new Config();
    QString strMe = pConfig->get_value("login-nick");
    delete pConfig;

    if (strNick == strMe)
    {
        if (strNewFlag == "+q") enable_topic();
        else if (strNewFlag == "-q") disable_topic();
        else if (strNewFlag == "+o") enable_topic();
        else if (strNewFlag == "-o") disable_topic();
        else if (strNewFlag == "+h") enable_topic();
        else if (strNewFlag == "-h") disable_topic();
        else if (strNewFlag == "+X") enable_moderation();
        else if (strNewFlag == "-X") disable_moderation();
    }
}

void TabWidget::update_nick(QString strNick)
{
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(strNick));
}

void TabWidget::clear_nicklist()
{
    iNickCount = 0;
    nickCount->setText(QString("Liczba osób: %1").arg(iNickCount));
    nick_flag.clear();
    nick_suffix.clear();
    nick_list->clear();
    nicklist.clear();
}

void TabWidget::set_open_channels(QStringList strOpenChannels)
{
    nick_list->set_open_channels(strOpenChannels);
}

void TabWidget::update_logo()
{
    if (mChannelAvatar->contains(strName) == true)
    {
        QPixmap pixmap;
        pixmap.loadFromData(mChannelAvatar->value(strName));
        logo->setPixmap(pixmap);
    }
}

// actions

void TabWidget::bold_clicked()
{
    if (bBold == true)
    {
        bold->setDown(false);
        bBold = false;
        strFontWeight = "";
    }
    else
    {
        bold->setDown(true);
        bBold = true;
        strFontWeight = "bold";
    }

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::italic_clicked()
{
    if (bItalic == true)
    {
        italic->setDown(false);
        bItalic = false;
        strFontStyle = "";
    }
    else
    {
        italic->setDown(true);
        bItalic = true;
        strFontStyle = "italic";
    }

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::arial_triggered()
{
    fontfamily->setText("Arial");
    strFontFamily = "Arial";

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::times_triggered()
{
    fontfamily->setText("Times");
    strFontFamily = "Times";

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::verdana_triggered()
{
    fontfamily->setText("Verdana");
    strFontFamily = "Verdana";

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::tahoma_triggered()
{
    fontfamily->setText("Tahoma");
    strFontFamily = "Tahoma";

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::courier_triggered()
{
    fontfamily->setText("Courier");
    strFontFamily = "Courier";

    int iWeight = (bBold == true ? 75 : 50);
    inputline->setFont(QFont(strFontFamily, -1, iWeight, bItalic));
}

void TabWidget::size8_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:8px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "8px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size9_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:9px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "9px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size10_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:10px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "10px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size11_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:11px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "11px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size12_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:12px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "12px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size14_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:14px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "14px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size18_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:18px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "18px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size20_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:20px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "20px";
    size->setText("Czcionka:"+strFontSize.left(strFontSize.length()-2));
}

// color

void TabWidget::color_clicked(int index)
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

void TabWidget::send_message(bool bType)
{
    QString strTextO = inputline->text();
    QStringList strTextA = strTextO.split(QRegExp("(\n|\r)"));

    for (int i = 0; i < strTextA.count(); i++)
    {
        QString strText = strTextA[i];
        QString strTextOriginal = strText;
        strLast_msg = strText;

        Config *pConfig = new Config();
        QString strMe = pConfig->get_value("login-nick");
        delete pConfig;

        if ((strText[0] == '/') && (strText[1] != '/'))
        {
            if (strText[0] == '/')
                strText = strText.right(strText.length()-1);
            strTextOriginal = strText;
            QStringList strTextList = strText.split(" ");

            Commands *pCommands = new Commands(strName, strText, settings);
            strText = pCommands->execute();
            delete pCommands;

            if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
                display_msg(strText, 7);
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

                    if (strCurrentColor != "000000")
                        strTextDisplay = "%C"+strCurrentColor+"%"+strTextDisplay;
                    if ((weight != "") || (font != "verdana"))
                        strTextDisplay = "%F"+weight+":"+font+"%"+strTextDisplay;

                    strTextSend = convert_emots(strTextSend);
                    strTextSend = replace_emots(strTextSend);
                    strTextDisplay = convert_emots(strTextDisplay);
                    strTextDisplay = replace_emots(strTextDisplay);

                    QDateTime dt = QDateTime::currentDateTime();
                    QString strDT = dt.toString("[hh:mm:ss] ");

                    Log *l = new Log();
                    l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strTextDisplay));

                    display_message(QString("%1<%2> %3ACTION %4%5").arg(strDT).arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01"))), 0);
                    if (pNetwork->is_connected() == true)
                        pNetwork->send(strTextSend);
                }
            }
            else
            {
                if ((pNetwork->is_connected() == true) && (strText.length() > 0))
                    pNetwork->send(strText);
            }

            inputline->clear();
        }
        else if (strName != "Status")
        {
            if ((pNetwork->is_connected() == true) && (strText.length() > 0))
            {
                QString weight;
                QString font = fontfamily->text().toLower();

                if (bBold == true) weight += "b";
                if (bItalic == true) weight += "i";

                if (strCurrentColor != "000000")
                    strText = "%C"+strCurrentColor+"%"+strText;
                if ((weight != "") || (font != "verdana"))
                    strText = "%F"+weight+":"+font+"%"+strText;

                strText = convert_emots(strText);
                strText = replace_emots(strText);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                Log *l = new Log();

                if (bType == true)
                {
                    l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText));

                    strText = QString("PRIVMSG %1 :%2").arg(strName).arg(strText);
                    pNetwork->send(strText);
                    display_message(QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-10-strName.length())), 0);
                }
                else
                {
                    l->save(strName, QString("%1 *<%2> %3").arg(strDT).arg(strMe).arg(strText));

                    strText = QString("MODERNOTICE %1 :%2").arg(strName).arg(strText);
                    pNetwork->send(strText);
                    display_message(QString("%1 *<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-14-strName.length())), 6);
                }

                inputline->clear();
            }
        }
    }
}

void TabWidget::inputline_return_pressed()
{
    send_message(true);
}

void TabWidget::moder_button_clicked()
{
    send_message(false);
}

void TabWidget::channel_settings_clicked()
{
    if (pNetwork->is_connected() == true)
    {
        if (strName != "Status")
        {
            dlgchannel_settings->set_channel(strName);
            dlgchannel_settings->show();
        }
    }
}

void TabWidget::moderation_clicked()
{
    dlgmoderation->set_active_channel(strName);
    dlgmoderation->show();
}

void TabWidget::topic_return_pressed()
{
    QString strText = topic->text();
    pNetwork->send(QString("CS SET %1 TOPIC %2").arg(strName).arg(strText));
}

void TabWidget::change_scroll_position()
{
    textEdit->page()->mainFrame()->setScrollBarValue(Qt::Vertical, textEdit->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}

void TabWidget::keyPressEvent(QKeyEvent *e)
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
