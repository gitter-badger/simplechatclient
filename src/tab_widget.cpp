/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

    QString strBackgroundColor = settings->value("background_color").toString();
    strBackgroundColor.replace("&", "&amp;");
    strBackgroundColor.replace("<", "&lt;");
    strBackgroundColor.replace(">", "&gt;");
    strBackgroundColor.replace("\"", "");
    strBackgroundColor.replace("\'", "");
    strBackgroundColor.replace("#", "");
    strBackgroundColor.replace(";", "");
    strBackgroundColor.replace("%", "");

    iNickCount = 0;
    bCursorPositionChanged = false;
    strCurrentColor = "000000";
    strFontSize = "11px";
    strContentStart = "<html><body style=\"background-color:#"+strBackgroundColor+";\">";
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

    topRightUpWidget = new QWidget(this);
    topRightUpLayout = new QHBoxLayout();
    topRightUpLayout->setMargin(0);
    topRightUpLayout->setAlignment(Qt::AlignLeft);
    topRightUpLayout->addWidget(topic);
    topRightUpLayout->addWidget(topicButton);
    topRightUpWidget->setLayout(topRightUpLayout);

    topRightWidget = new QWidget(this);
    topRightLayout = new QVBoxLayout();
    topRightLayout->setMargin(0);
    topRightLayout->setAlignment(Qt::AlignTop);
    topRightLayout->addWidget(topRightUpWidget);
    topRightLayout->addWidget(topicDetails);
    topRightWidget->setLayout(topRightLayout);

    topLeftWidget = new QWidget(this);
    topLeftLayout = new QVBoxLayout();
    topLeftLayout->setMargin(0);
    topLeftLayout->setAlignment(Qt::AlignTop);
    topLeftLayout->addWidget(logo);
    topLeftWidget->setLayout(topLeftLayout);

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
    bMyBold = false;

    italic = new QPushButton(this);
    italic->setFont(QFont("Times New Roman", -1, -1, true));
    italic->setText("I");
    italic->setParent(this);
    italic->setFlat(true);
    italic->setCheckable(true);
    italic->setMaximumWidth(25);
    italic->setMaximumHeight(25);
    italic->show();
    bMyItalic = false;

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
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
    size->setParent(this);
    size->setMaximumWidth(250);
    size->setMaximumHeight(25);
    size->setMenu(sizeMenu);
    size->show();

    color = new QComboBox(this);
    color->setParent(this);
    color->setIconSize(QSize(50,10));

    QStringList comboBoxColors;
    comboBoxColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    int iComboBoxColors = 0;
    foreach (QString strColor, comboBoxColors)
    {
        QPixmap pixmap(50,10);
        pixmap.fill(QColor(strColor));
        color->insertItem(iComboBoxColors, pixmap, "");
        iComboBoxColors++;
    }
    color->show();

    channel_settings = new QPushButton(this);
    channel_settings->setText(tr("Settings"));
    channel_settings->setParent(this);
    channel_settings->show();

    moderation = new QPushButton(this);
    moderation->setText(tr("Moderation"));
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
    sendButton->setText(tr("Send"));
    sendButton->setParent(this);
    sendButton->setMaximumWidth(70);
    sendButton->setMaximumHeight(25);
    sendButton->show();

    moderSendButton = new QPushButton();
    moderSendButton->setText(tr("Send to moderators"));
    moderSendButton->setParent(this);
    moderSendButton->setMaximumWidth(180);
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
        topRightUpLayout->removeWidget(topicButton);
        moderation->hide();
        toolLayout->removeWidget(moderation);
        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        if (settings->value("style") == "modern")
        {
            rightLayout->addWidget(webLink);
            rightLayout->addWidget(nickCount);
        }
        else if (settings->value("style") == "classic")
        {
            webLink->hide();
            nickCount->hide();
            topicDetails->hide();
            topLeftWidget->hide();
            rightLayout->setMargin(0);
            leftLayout->setMargin(0);
        }

        rightLayout->addWidget(nick_list);

        leftLayout->addWidget(topWidget);
        leftLayout->addWidget(textEdit);
        leftLayout->addWidget(toolWidget);
        leftLayout->addWidget(bottomWidget);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);

        if (settings->value("style") == "classic")
            mainLayout->setMargin(0);

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

    if (settings->value("disable_logs").toString() == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

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

    if (settings->value("disable_logs").toString() == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings->value("hide_join_part").toString() == "on")
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_message(QString strData, int iLevel)
{
// fix max size
    if (strContent.count("</p>") > 200)
    {
        QStringList list = strContent.split("</p>");
        int iCount = strContent.count("</p>");
        strContent.clear();
        for (int i = iCount-200; i < iCount; i++)
            strContent.append(list.at(i)+"</p>");
        list.clear();
    }

// fix data
    strData += " ";
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

// content last
    QString strContentLast;

// colors
    QString strFontColor;

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
    else
        strFontColor = "#000000"; // default black

    strData.insert(11, "<span style=\"color:"+strFontColor+";\">");
    strContentLast = "</span>"+strContentLast;

// if /me remove time,action <>
    if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
    {
        strData = strData.right(strData.length() - 11);
        if (strData.indexOf("ACTION ") != -1) strData = strData.replace("ACTION ", QString::null);
        if (strData.indexOf("&lt;") != -1) strData = strData.remove(strData.indexOf("&lt;"),4);
        if (strData.indexOf("&gt;") != -1) strData = strData.remove(strData.indexOf("&gt;"),4);
    }

// colors
    if (settings->value("hide_formating").toString() == "off")
    {
        int iCount = strData.count("%C");

        strData.replace("%C000000%", "<span style=\"color:#000000;\">");
        strData.replace("%C623c00%", "<span style=\"color:#623c00;\">");
        strData.replace("%Cc86c00%", "<span style=\"color:#c86c00;\">");
        strData.replace("%Cff6500%", "<span style=\"color:#ff6500;\">");
        strData.replace("%Cff0000%", "<span style=\"color:#ff0000;\">");
        strData.replace("%Ce40f0f%", "<span style=\"color:#e40f0f;\">");
        strData.replace("%C990033%", "<span style=\"color:#990033;\">");
        strData.replace("%C8800ab%", "<span style=\"color:#8800ab;\">");
        strData.replace("%Cce00ff%", "<span style=\"color:#ce00ff;\">");
        strData.replace("%C0f2ab1%", "<span style=\"color:#0f2ab1;\">");
        strData.replace("%C3030ce%", "<span style=\"color:#3030ce;\">");
        strData.replace("%C006699%", "<span style=\"color:#006699;\">");
        strData.replace("%C1a866e%", "<span style=\"color:#1a866e;\">");
        strData.replace("%C008100%", "<span style=\"color:#008100;\">");
        strData.replace("%C959595%", "<span style=\"color:#959595;\">");

        QString strSpan;
        for (int i = 0; i < iCount; i++)
            strSpan += "</span>";

        strContentLast = strSpan+strContentLast;
    }

// emoticons
    if (strData.indexOf("%I") != -1)
    {
        QString strPath = QCoreApplication::applicationDirPath();

        while (strData.indexOf("%I") != -1)
        {
            int iStartPos = strData.indexOf("%I");
            int iEndPos = strData.indexOf("%", iStartPos+1);
            if (iEndPos != -1)
            {
                iEndPos++;
                QString strEmoticonFull = strData.mid(iStartPos, iEndPos-iStartPos);
                QString strEmoticon = strEmoticonFull.mid(2,strEmoticonFull.length()-3);
                QString strInsert;

                QString strEmoticonFull1 = strPath+"/3rdparty/emoticons/"+strEmoticon+".gif";
                QString strEmoticonFull2 = strPath+"/3rdparty/emoticons_other/"+strEmoticon+".gif";
                QFile f1(strEmoticonFull1);
                QFile f2(strEmoticonFull2);
                if ((f1.exists() == true) && (settings->value("hide_formating").toString() == "off"))
                {
        #ifdef Q_WS_X11
                    strInsert = "<img src=\"file://"+strEmoticonFull1+"\" alt=\""+strEmoticon+"\" />";
        #else
                    strInsert = "<img src=\""+strEmoticonFull1+"\" alt=\""+strEmoticon+"\" />";
        #endif
                }
                else if ((f2.exists() == true) && (settings->value("hide_formating").toString() == "off"))
                {
        #ifdef Q_WS_X11
                    strInsert = "<img src=\"file://"+strEmoticonFull2+"\" alt=\""+strEmoticon+"\" />";
        #else
                    strInsert = "<img src=\""+strEmoticonFull2+"\" alt=\""+strEmoticon+"\" />";
        #endif
                }
                // emoticon not exist or hide formating
                else
                    strInsert = "//"+strEmoticon;

                strData.replace(strEmoticonFull, strInsert);
            }
            else
                break;
        }
    }

// fonts
    if (strData.indexOf("%F") != -1)
    {
        int iCount = strData.count("%F");

        while (strData.indexOf("%F") != -1)
        {
            int iStartPos = strData.indexOf("%F");
            int iEndPos = strData.indexOf("%", iStartPos+1);
            if (iEndPos != -1)
            {
                iEndPos++;
                QString strFontFull = strData.mid(iStartPos, iEndPos-iStartPos);
                QString strFont = strFontFull.mid(2,strFontFull.length()-3);
                QString strInsert;

                QString strFontStyle = "normal";
                QString strFontFamily = "Verdana";
                QString strFontWeight = "normal";
                QString strFontName = "Verdana";

                if (strFont.indexOf(":") != -1)
                {
                    strFontWeight = strFont.left(strFont.indexOf(":"));
                    strFontName = strFont.right(strFont.length()-strFont.indexOf(":")-1);

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

                     strInsert = "<span style=\"font-weight:"+strFontWeight+";font-style:"+strFontStyle+";font-family:"+strFontFamily+";\">";
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
                     strInsert = "<span style=\"font-weight:"+strFontWeight+";font-style:"+strFontStyle+";font-family:"+strFontFamily+";\">";
                }

                strData.replace(strFontFull, strInsert);
            }
            else
                break;
        }

        QString strSpan;
        for (int i = 0; i < iCount; i++)
            strSpan += "</span>";

        strContentLast = strSpan+strContentLast;
    }

// hilight
    if (iLevel == 8)
    {
        strData.append("<hr>");

        if (settings->value("disable_sounds").toString() == "off")
            pNotify->play();
    }

// init text
    strContent.append("<p style=\"margin:0;padding:0;font-style:normal;color:#000000;text-align:left;font-family:Verdana;font-weight:normal;font-size:"+strFontSize+";\">");

// /me
    if (settings->value("hide_formating").toString() == "off")
    {
        if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
        {
            int iLast1 = strContent.lastIndexOf("text-align:");
            int iLast2 = strContent.indexOf(";",iLast1);
            strContent.remove(iLast1, iLast2-iLast1);
            strContent.insert(iLast1, "text-align:center");

            strData.replace(QString(QByteArray("\x01")), "");
        }
    }

// text
    strContent.append(strData);
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

    if (settings->value("style") == "modern")
        topic->setToolTip(strTopic);
    else if (settings->value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::enable_topic()
{
    topic->setReadOnly(false);
    topicButton->show();
    topRightUpLayout->addWidget(topicButton);
}

void TabWidget::disable_topic()
{
    topic->setReadOnly(true);
    topicButton->hide();
    topRightUpLayout->removeWidget(topicButton);
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
    topicDetails->setText(QString(tr("Topic set by %1")).arg(strAuthor));

    if (settings->value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::set_link(QString strUrl)
{
    webLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >"+tr("Channel website")+"</a>").arg(strUrl));
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
        nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
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
        nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
    }
}

void TabWidget::nicklist_add(QString strNick, QString strStatus)
{
    nick_list->nicklist_add(strNick, strStatus, &nicklist);
}

void TabWidget::nicklist_remove(QString strNick)
{
    nick_list->nicklist_remove(strNick, &nicklist, &nick_status);
}

bool TabWidget::nicklist_exist(QString strNick)
{
    return nick_list->nicklist_exist(strNick, &nicklist);
}

void TabWidget::nicklist_clear()
{
    nick_list->nicklist_clear(&nicklist, &nick_status);
}

void TabWidget::nicklist_refresh()
{
    nick_list->nicklist_refresh(&nicklist, &nick_status);
}

void TabWidget::nicklist_refresh_all()
{
    nick_list->nicklist_refresh(&nicklist, &nick_status);
    inputline->set_userslist(nick_list);
}

void TabWidget::nicklist_sort()
{
    nick_list->nicklist_sort(&nicklist, &nick_status);
}

void TabWidget::nicklist_quicksort(QString strStatus)
{
    nick_list->nicklist_quicksort(strStatus, &nicklist, &nick_status);
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
    nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
    nick_flag.clear();
    nick_suffix.clear();
    nick_list->clear();
    nicklist.clear();
}

void TabWidget::set_open_channels(QStringList strOpenChannels)
{
    nick_list->set_open_channels(strOpenChannels);
}

void TabWidget::update_nick_avatar()
{
    nick_list->nicklist_refresh(&nicklist, &nick_status);
}

void TabWidget::update_channel_avatar()
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
    if (bMyBold == true)
    {
        bold->setDown(false);
        bMyBold = false;
        strMyFontWeight = "";
    }
    else
    {
        bold->setDown(true);
        bMyBold = true;
        strMyFontWeight = "bold";
    }

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::italic_clicked()
{
    if (bMyItalic == true)
    {
        italic->setDown(false);
        bMyItalic = false;
        strMyFontStyle = "";
    }
    else
    {
        italic->setDown(true);
        bMyItalic = true;
        strMyFontStyle = "italic";
    }

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::arial_triggered()
{
    fontfamily->setText("Arial");
    strMyFontFamily = "Arial";

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::times_triggered()
{
    fontfamily->setText("Times");
    strMyFontFamily = "Times";

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::verdana_triggered()
{
    fontfamily->setText("Verdana");
    strMyFontFamily = "Verdana";

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::tahoma_triggered()
{
    fontfamily->setText("Tahoma");
    strMyFontFamily = "Tahoma";

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::courier_triggered()
{
    fontfamily->setText("Courier");
    strMyFontFamily = "Courier";

    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void TabWidget::size8_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:8px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "8px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size9_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:9px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "9px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size10_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:10px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "10px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size11_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:11px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "11px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size12_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:12px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "12px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size14_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:14px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "14px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size18_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:18px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "18px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
}

void TabWidget::size20_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:20px");
    textEdit->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "20px";
    size->setText(tr("Font:")+strFontSize.left(strFontSize.length()-2));
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

                    if (bMyBold == true) weight += "b";
                    if (bMyItalic == true) weight += "i";

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

                    if (settings->value("disable_logs").toString() == "off")
                    {
                        Log *l = new Log();
                        l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strTextDisplay));
                        delete l;
                    }

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

                if (bMyBold == true) weight += "b";
                if (bMyItalic == true) weight += "i";

                if (strCurrentColor != "000000")
                    strText = "%C"+strCurrentColor+"%"+strText;
                if ((weight != "") || (font != "verdana"))
                    strText = "%F"+weight+":"+font+"%"+strText;

                strText = convert_emots(strText);
                strText = replace_emots(strText);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                if (bType == true)
                {
                    if (settings->value("disable_logs").toString() == "off")
                    {
                        Log *l = new Log();
                        l->save(strName, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText));
                        delete l;
                    }

                    strText = QString("PRIVMSG %1 :%2").arg(strName).arg(strText);
                    pNetwork->send(strText);
                    display_message(QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-10-strName.length())), 0);
                }
                else
                {
                    if (settings->value("disable_logs").toString() == "off")
                    {
                        Log *l = new Log();
                        l->save(strName, QString("%1 *<%2> %3").arg(strDT).arg(strMe).arg(strText));
                        delete l;
                    }

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
