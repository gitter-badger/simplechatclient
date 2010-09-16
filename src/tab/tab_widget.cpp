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

TabWidget::TabWidget(QWidget *parent, Network *param1, QString param2, Notify *param3, QMap <QString, QByteArray> *param4, QMap <QString, QByteArray> *param5, DlgChannelSettings *param6, DlgModeration *param7, QTcpSocket *param8)
{
    myparent = parent;
    pNetwork = param1;
    strName = param2;
    pNotify = param3;
    mNickAvatar = param4;
    mChannelAvatar = param5;
    dlgchannel_settings = param6;
    dlgmoderation = param7;
    camSocket = param8;

    QSettings settings;
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());

    iNickCount = 0;
    bCursorPositionChanged = false;
    strCurrentColor = "#000000";
    strFontSize = "11px";
    strContentStart = "<html><body style=\"background-color:"+strBackgroundColor+";\">";
    strContentEnd = "</body></html>";

    splitter = new QSplitter(this);
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    leftWidget = new QWidget(this);
    rightWidget = new QWidget(this);

    topic = new QWebView(this);
    topic->setParent(this);
    topic->setMinimumHeight(30);
    topic->setMinimumWidth(16777215);
    topic->setMaximumHeight(30);
    topic->setMaximumWidth(16777215);
    topic->show();

    topicDetails = new QLabel();
    topicDetails->setParent(this);
    topicDetails->setOpenExternalLinks(false);
    topicDetails->setAlignment(Qt::AlignLeft);
    topicDetails->show();

    logo = new QLabel(this);
    logo->setParent(this);
    logo->show();

    topRightWidget = new QWidget(this);
    topRightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    topRightLayout = new QVBoxLayout();
    topRightLayout->setMargin(0);
    topRightLayout->setAlignment(Qt::AlignTop);
    topRightLayout->addWidget(topic);
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

    nick_list = new Nicklist(myparent, pNetwork, strName, mNickAvatar, camSocket);
    nick_list->setParent(this);
    //nick_list->setItemDelegate(new NicklistDelegate(nick_list));
    nick_list->show();

    mainWebView = new MainWebView(myparent, pNetwork, strName, camSocket);
    mainWebView->setParent(this);
    mainWebView->show();

    bold = new QPushButton(this);
    bold->setParent(this);
    bold->setToolTip(tr("Bold"));
    bold->setFont(QFont("Times New Roman", -1, 75, false));
#ifdef Q_WS_X11
    bold->setText("b");
#else
    bold->setText("B");
#endif
    bold->setFlat(true);
    bold->setCheckable(true);
    bold->setMaximumWidth(25);
    bold->setMaximumHeight(25);
    bold->show();
    bMyBold = false;

    italic = new QPushButton(this);
    italic->setParent(this);
    italic->setToolTip(tr("Italic"));
    italic->setFont(QFont("Times New Roman", -1, -1, true));
    italic->setText("I");
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

    fontMenu = new QMenu(this);
    fontMenu->addAction(arialAct);
    fontMenu->addAction(timesAct);
    fontMenu->addAction(verdanaAct);
    fontMenu->addAction(tahomaAct);
    fontMenu->addAction(courierAct);

    fontfamily = new QPushButton(this);
    fontfamily->setParent(this);
    fontfamily->setToolTip(tr("Font family"));
    fontfamily->setFont(QFont("Verdana", -1, -1, false));
    fontfamily->setText("Verdana");
    fontfamily->setMaximumWidth(250);
    fontfamily->setMaximumHeight(25);
    fontfamily->setMenu(fontMenu);
    fontfamily->show();

    color = new QComboBox(this);
    color->setParent(this);
    color->setToolTip(tr("Font color"));
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

    sizeMenu = new QMenu(this);
    sizeMenu->addAction(size8Act);
    sizeMenu->addAction(size9Act);
    sizeMenu->addAction(size10Act);
    sizeMenu->addAction(size11Act);
    sizeMenu->addAction(size12Act);
    sizeMenu->addAction(size14Act);
    sizeMenu->addAction(size18Act);
    sizeMenu->addAction(size20Act);

    size = new QPushButton(this);
    size->setParent(this);
    size->setToolTip(tr("Font size"));
    size->setFont(QFont("Times New Roman", -1, -1, false));
    size->setText("F");
    size->setMaximumWidth(45);
    size->setMaximumHeight(25);
    size->setMenu(sizeMenu);
    size->show();

    emoticons = new QPushButton(QIcon(":/images/logo_64.png"), "", this);
    emoticons->setParent(this);
    emoticons->setToolTip(tr("Emoticons"));
    emoticons->setMaximumWidth(25);
    emoticons->setMaximumHeight(25);
    emoticons->show();

    separator = new QLabel();
    separator->setParent(this);
    separator->setText(" | ");
    separator->setEnabled(false);
    separator->show();

    channel_settings = new QPushButton(this);
    channel_settings->setParent(this);
    channel_settings->setToolTip(tr("Channel settings"));
    channel_settings->setText(tr("Settings"));
    channel_settings->show();

    moderation = new QPushButton(this);
    moderation->setParent(this);
    moderation->setToolTip(tr("Moderation"));
    moderation->setText(tr("Moderation"));
    moderation->show();

    clear = new QPushButton(QIcon(":/images/clear.png"), "", this);
    clear->setParent(this);
    clear->setToolTip(tr("Clear"));
    clear->setMaximumWidth(25);
    clear->setMaximumHeight(25);
    clear->show();

    scroll = new QPushButton(QIcon(":/images/arrow.png"), "", this);
    scroll->setParent(this);
    scroll->setToolTip(tr("Scroll"));
    scroll->setMaximumWidth(25);
    scroll->setMaximumHeight(25);
    scroll->setFlat(true);
    scroll->setCheckable(true);
    scroll->show();
    bScroll = true;

    toolWidget = new QWidget(this);
    toolLayout = new QHBoxLayout();
    toolLayout->setMargin(0);
    toolLayout->setAlignment(Qt::AlignLeft);
    toolLayout->addWidget(bold);
    toolLayout->addWidget(italic);
    toolLayout->addWidget(fontfamily);
    toolLayout->addWidget(color);
    toolLayout->addWidget(size);
    toolLayout->addWidget(emoticons);
    toolLayout->addWidget(separator);
    toolLayout->addWidget(channel_settings);
    toolLayout->addWidget(moderation);
    toolLayout->addWidget(clear);
    toolLayout->addWidget(scroll);
    toolWidget->setLayout(toolLayout);

    nickLabel = new QLabel();
    nickLabel->setParent(this);
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(tr("(Unregistered)")));
    nickLabel->show();

    inputline = new Inputline(this);
    inputline->setParent(this);
    inputline->setMinimumWidth(400);
    inputline->setMaxLength(300);
    inputline->setFont(QFont("Verdana", -1, -1, false));
    inputline->show();

    sendButton = new QPushButton();
    sendButton->setParent(this);
    sendButton->setToolTip(tr("Send"));
    sendButton->setText(tr("Send"));
    sendButton->setMaximumWidth(70);
    sendButton->setMaximumHeight(25);
    sendButton->show();

    moderSendButton = new QPushButton();
    moderSendButton->setParent(this);
    moderSendButton->setToolTip(tr("Send to moderators"));
    moderSendButton->setText(tr("Send to moderators"));
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
        moderation->hide();
        toolLayout->removeWidget(moderation);
        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        if (settings.value("style") == "modern")
        {
            rightLayout->addWidget(webLink);
            rightLayout->addWidget(nickCount);
        }
        else if (settings.value("style") == "classic")
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
        leftLayout->addWidget(mainWebView);
        leftLayout->addWidget(toolWidget);
        leftLayout->addWidget(bottomWidget);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);

        mainLayout->setMargin(0);
        mainLayout->addWidget(splitter);
    }
    else if (strName[0] == '^')
    {
        logo->hide();
        topic->hide();
        topicDetails->hide();
        topRightWidget->hide();
        topLeftWidget->hide();
        topWidget->hide();

        webLink->hide();
        nickCount->hide();

        moderation->hide();
        toolLayout->removeWidget(moderation);
        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        rightLayout->addWidget(nick_list);

        leftLayout->addWidget(mainWebView);
        leftLayout->addWidget(toolWidget);
        leftLayout->addWidget(bottomWidget);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);

        mainLayout->setMargin(0);
        mainLayout->addWidget(splitter);
    }
    else
    {
        logo->hide();
        topic->hide();
        topicDetails->hide();
        topRightWidget->hide();
        topLeftWidget->hide();
        topWidget->hide();

        webLink->hide();
        nickCount->hide();
        nick_list->hide();

        bold->hide();
        italic->hide();
        fontfamily->hide();
        color->hide();
        size->hide();
        emoticons->hide();
        separator->hide();
        channel_settings->hide();
        moderation->hide();
        clear->hide();
        scroll->hide();
        toolWidget->hide();

        moderSendButton->hide();
        bottomLayout->removeWidget(moderSendButton);

        leftLayout->addWidget(mainWebView);
        leftLayout->addWidget(bottomWidget);

        leftWidget->setLayout(leftLayout);
        rightWidget->setLayout(rightLayout);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);

        mainLayout->setMargin(0);
        mainLayout->addWidget(splitter);
    }

    if (strName == "Status") channel_settings->hide();
    this->setLayout(mainLayout);

// set default font
    set_default();

// set colors
    inputline->setStyleSheet(QString("color:%1;").arg(strDefaultFontColor));

    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);

// signals
    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputline_return_pressed()));
    QObject::connect(inputline, SIGNAL(returnPressed()), this, SLOT(inputline_return_pressed()));
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
    QObject::connect(emoticons, SIGNAL(clicked()), this, SLOT(emoticons_clicked()));
    QObject::connect(channel_settings, SIGNAL(clicked()), this, SLOT(channel_settings_clicked()));
    QObject::connect(moderation, SIGNAL(clicked()), this, SLOT(moderation_clicked()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moder_button_clicked()));
    QObject::connect(clear, SIGNAL(clicked()), this, SLOT(clear_clicked()));
    QObject::connect(scroll, SIGNAL(clicked()), this, SLOT(scroll_clicked()));

    QObject::connect(mainWebView, SIGNAL(loadFinished(bool)), this, SLOT(change_scroll_position()));
}

TabWidget::~TabWidget()
{
    nick_status.clear();
    nickLabel->clear();
    strContent.clear();
    mainWebView->setHtml(strContent,QUrl(""));

    delete nick_list;
    delete inputline;
}

void TabWidget::set_default()
{
    QSettings settings;

// set default bold
    if (settings.value("my_bold").toString() == "on")
    {
        bold->setChecked(true);
        bMyBold = true;
        strMyFontWeight = "bold";
    }
    else
    {
        bold->setChecked(false);
        bMyBold = false;
        strMyFontWeight = "";
    }

// set default italic
    if (settings.value("my_italic").toString() == "on")
    {
        italic->setChecked(true);
        bMyItalic = true;
        strMyFontStyle = "italic";
    }
    else
    {
        italic->setChecked(false);
        bMyItalic = false;
        strMyFontStyle = "";
    }

// refresh bold italic
    int iWeight = (bMyBold == true ? 75 : 50);
    inputline->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));

// set default font
    QString strMyFont = settings.value("my_font").toString();
    fontfamily->setText(strMyFont);
    strMyFontFamily = strMyFont;

// set default color
    QString strMyColor = settings.value("my_color").toString();
    strCurrentColor = strMyColor;
    inputline->setStyleSheet(QString("color: "+strCurrentColor));

    int iMyColor;
    if (strMyColor == "#000000") iMyColor = 0;
    else if (strMyColor == "#623c00") iMyColor = 1;
    else if (strMyColor == "#c86c00") iMyColor = 2;
    else if (strMyColor == "#ff6500") iMyColor = 3;
    else if (strMyColor == "#ff0000") iMyColor = 4;
    else if (strMyColor == "#e40f0f") iMyColor = 5;
    else if (strMyColor == "#990033") iMyColor = 6;
    else if (strMyColor == "#8800ab") iMyColor = 7;
    else if (strMyColor == "#ce00ff") iMyColor = 8;
    else if (strMyColor == "#0f2ab1") iMyColor = 9;
    else if (strMyColor == "#3030ce") iMyColor = 10;
    else if (strMyColor == "#006699") iMyColor = 11;
    else if (strMyColor == "#1a866e") iMyColor = 12;
    else if (strMyColor == "#008100") iMyColor = 13;
    else if (strMyColor == "#959595") iMyColor = 14;
    else
        iMyColor = 0;

    color->setCurrentIndex(iMyColor);

// set default scroll
    scroll->setChecked(true);
    bScroll = true;
}

QString TabWidget::addslashes(QString strData)
{
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    strData.replace("\"", "");
    strData.replace("\'", "");
    strData.replace(";", "");
    strData.replace("%", "");

    if (strData.isEmpty() == true)
        return "#000000";
    else
        return strData;
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

    QSettings settings;
    if (settings.value("disable_logs").toString() == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings.value("hide_join_part").toString() == "on")
            return;
        if ((settings.value("hide_join_part_200").toString() == "on") && (iNickCount > 200))
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_msg(QString strData, int iLevel)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    QSettings settings;
    if (settings.value("disable_logs").toString() == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings.value("hide_join_part").toString() == "on")
            return;
        if ((settings.value("hide_join_part_200").toString() == "on") && (iNickCount > 200))
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_message(QString strData, int iLevel)
{
    QSettings settings;

// fix max size
    if (strContent.count("</p>") > 150)
    {
        QStringList list = strContent.split("</p>");
        int iCount = strContent.count("</p>");
        strContent.clear();
        for (int i = iCount-150; i < iCount; i++)
            strContent.append(list.at(i)+"</p>");
        list.clear();
    }

// fix data
    strData += " ";
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    // channels
    strData.replace(QRegExp("#([~-_a-zA-Z0-9\xa1\xaf\xa6\xac\xca\xc6\xd1\xd3\xa3\xb1\xbf\xb6\xbc\xea\xe6\xf1\xf3\xb3]+)"), "<a id=\"level_chan\" style=\"color:"+addslashes(settings.value("channel_font_color").toString())+";text-decoration:none;\" href=\"chan#\\1\">#\\1</a>");
    // nicks
    strData.replace(QRegExp("&lt;([~-_a-zA-Z0-9\xa1\xaf\xa6\xac\xca\xc6\xd1\xd3\xa3\xb1\xbf\xb6\xbc\xea\xe6\xf1\xf3\xb3]+)&gt;"), "<a id=\"level_0\" style=\"color:"+addslashes(settings.value("default_font_color").toString())+";text-decoration:none;\" href=\"nick\\1\">&lt;\\1&gt;</a>");

// content last
    QString strContentLast;

// colors
    QString strFontColor;

    if (iLevel == 0)
        strFontColor = addslashes(settings.value("default_font_color").toString()); // default black
    else if (iLevel == 1) // join
        strFontColor = addslashes(settings.value("font_color_level_1").toString()); // default green
    else if (iLevel == 2) // part
        strFontColor = addslashes(settings.value("font_color_level_2").toString()); // default light blue
    else if (iLevel == 3) // quit
        strFontColor = addslashes(settings.value("font_color_level_3").toString()); // default dark blue
    else if (iLevel == 4) // kick
        strFontColor = addslashes(settings.value("font_color_level_4").toString()); // default dark blue
    else if (iLevel == 5) // mode
        strFontColor = addslashes(settings.value("font_color_level_5").toString()); // default green
    else if (iLevel == 6) // notice
        strFontColor = addslashes(settings.value("font_color_level_6").toString()); // default blue
    else if (iLevel == 7) // info
        strFontColor = addslashes(settings.value("font_color_level_7").toString()); // default gray
    else if (iLevel == 8) // hilight no color
        strFontColor = addslashes(settings.value("default_font_color").toString()); // default black
    else if (iLevel == 9) // error
        strFontColor = addslashes(settings.value("font_color_level_9").toString()); // default red
    else
    {
        iLevel = 0;
        strFontColor = addslashes(settings.value("default_font_color").toString()); // default black
    }

    strData.insert(11, "<span id=\"level_"+QString::number(iLevel)+"\" style=\"color:"+strFontColor+";\">");
    strContentLast = "</span>"+strContentLast;

// if /me remove time,action <>
    if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
    {
        strData = strData.right(strData.length() - 11);
        if (strData.indexOf("ACTION ") != -1) strData = strData.replace("ACTION ", QString::null);
        if (strData.indexOf("&lt;") != -1) strData = strData.remove(strData.indexOf("&lt;"),4);
        if (strData.indexOf("&gt;") != -1) strData = strData.remove(strData.indexOf("&gt;"),4);
    }

// convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convert_text(&strData, &strContentLast);
    delete convertText;

// hilight
    QString strTextDecoration = "none";
    if (iLevel == 8)
    {
        strTextDecoration = "underline";

        if (settings.value("disable_sounds").toString() == "off")
            pNotify->play();
    }

// /me
    QString strTextAlign = "left";
    if (settings.value("hide_formating").toString() == "off")
    {
        if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
        {
            strTextAlign = "center";
            strData.replace(QString(QByteArray("\x01")), "");
        }
    }

// init text
    strContent.append("<p id=\"level_0\" style=\"color:"+addslashes(settings.value("default_font_color").toString())+";margin:0;padding:0;font-style:normal;text-align:"+strTextAlign+";font-family:Verdana;font-weight:normal;font-size:"+strFontSize+";text-decoration:"+strTextDecoration+";\">");

// text
    strContent.append(strData);
    strContent = strContent+strContentLast;
    strContent.append("</p>");

    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
}

// window options

void TabWidget::set_topic(QString strTopic)
{
    QString strData = strTopic;

// replace
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

// colors
    QSettings settings;
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());

// content last
    QString strContentLast;
    QString strContentStart = "<html><body style=\"margin:0;padding:0;font-style:normal;color:"+strDefaultFontColor+";text-align:left;font-family:Verdana;font-weight:normal;font-size:12px;background-color:"+strBackgroundColor+";\">";
    QString strContentEnd = "</body></html>";

// convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convert_text(&strData, &strContentLast);
    delete convertText;

// init text
    strTopicContent = strData;
    strTopicContent = strTopicContent+strContentLast;

// set topic
    topic->setHtml(strContentStart+strTopicContent+strContentEnd,QUrl(""));

// tooltip
    strTopic.replace(QRegExp("%C(\\S+)%"),"");
    strTopic.replace(QRegExp("%F(\\S+)%"),"");
    strTopic.replace(QRegExp("%I(\\S+)%"),"<\\1>");

    if (settings.value("style") == "modern")
        topic->setToolTip(strTopic);
    else if (settings.value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::enable_topic()
{
}

void TabWidget::disable_topic()
{
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

    QSettings settings;
    if (settings.value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::set_link(QString strUrl)
{
    webLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >"+tr("Channel website")+"</a>").arg(strUrl));
    webLink->setToolTip(strUrl);
}

// nick list

void TabWidget::add_user(QString strNick, QString strStatus)
{
    if (nicklist_exist(strNick) == false)
    {
        nicklist_add(strNick, strStatus);
        inputline->set_userslist(nick_list);

        iNickCount++;
        nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
    }
}

void TabWidget::del_user(QString strNick)
{
    if (nicklist_exist(strNick) == true)
    {
        nicklist_remove(strNick);
        inputline->set_userslist(nick_list);

        iNickCount--;
        nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
    }
}

void TabWidget::replace_color(QString level, QString color)
{
    strContent.replace(QRegExp(QString("id=\"level_%1\" style=\"color:#(......);").arg(level)), QString("id=\"level_%1\" style=\"color:%2;").arg(level).arg(color));
}

void TabWidget::nicklist_add(QString strNick, QString strStatus)
{
    nick_list->nicklist_add(strNick, strStatus, &nick_status);
}

void TabWidget::nicklist_remove(QString strNick)
{
    nick_list->nicklist_remove(strNick, &nick_status);
}

bool TabWidget::nicklist_exist(QString strNick)
{
    return nick_list->nicklist_exist(strNick, &nick_status);
}

void TabWidget::nicklist_refresh_all()
{
    //raw 366: End of /NAMES list.
    nick_list->expandAll();
    inputline->set_userslist(nick_list);
}

QStringList TabWidget::get_nicklist()
{
    return nick_list->nicklist_get(&nick_status);
}

void TabWidget::change_flag(QString strNick, QString strNewFlag)
{
    NickStatus ns;
    QString strStatus = ns.status;

    if ((strNewFlag == "+q") && (strStatus.indexOf("`") == -1)) strStatus.append("`");
    else if ((strNewFlag == "-q") && (strStatus.indexOf("`") != -1)) strStatus.remove("`");
    else if ((strNewFlag == "+o") && (strStatus.indexOf("@") == -1)) strStatus.append("@");
    else if ((strNewFlag == "-o") && (strStatus.indexOf("@") != -1)) strStatus.remove("@");
    else if ((strNewFlag == "+h") && (strStatus.indexOf("%") == -1)) strStatus.append("%");
    else if ((strNewFlag == "-h") && (strStatus.indexOf("%") != -1)) strStatus.remove("%");
    else if ((strNewFlag == "+v") && (strStatus.indexOf("+") == -1)) strStatus.append("+");
    else if ((strNewFlag == "-v") && (strStatus.indexOf("+") != -1)) strStatus.remove("+");
    else if ((strNewFlag == "+X") && (strStatus.indexOf("!") == -1)) strStatus.append("!");
    else if ((strNewFlag == "-X") && (strStatus.indexOf("!") != -1)) strStatus.remove("!");
    else if ((strNewFlag == "+Y") && (strStatus.indexOf("=") == -1)) strStatus.append("=");
    else if ((strNewFlag == "-Y") && (strStatus.indexOf("=") != -1)) strStatus.remove("=");
    else if ((strNewFlag == "+O") && (strStatus.indexOf("O") == -1)) strStatus.append("O");
    else if ((strNewFlag == "-O") && (strStatus.indexOf("O") != -1)) strStatus.remove("O");
    else if ((strNewFlag == "+b") && (strStatus.indexOf("b") == -1)) strStatus.append("b");
    else if ((strNewFlag == "-b") && (strStatus.indexOf("b") != -1)) strStatus.remove("b");
    else if ((strNewFlag == "+r") && (strStatus.indexOf("r") == -1)) strStatus.append("r");
    else if ((strNewFlag == "-r") && (strStatus.indexOf("r") != -1)) strStatus.remove("r");
    else if ((strNewFlag == "+W") && (strStatus.indexOf("W") == -1)) strStatus.append("W");
    else if ((strNewFlag == "-W") && (strStatus.indexOf("W") != -1)) strStatus.remove("W");
    else if ((strNewFlag == "+V") && (strStatus.indexOf("V") == -1)) strStatus.append("V");
    else if ((strNewFlag == "-V") && (strStatus.indexOf("V") != -1)) strStatus.remove("V");
    else if ((strNewFlag == "+x") && (strStatus.indexOf("x") == -1)) strStatus.append("x");
    else if ((strNewFlag == "-x") && (strStatus.indexOf("x") != -1)) strStatus.remove("x");

    del_user(strNick);
    add_user(strNick, strStatus);

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
    nick_status.clear();
    nick_list->clear();
}

void TabWidget::set_user_info(QString strNick, QString strKey, QString strValue)
{
    nick_list->set_user_info(strNick, strKey, strValue);
    mainWebView->set_user_info(strNick, strKey, strValue);
}

void TabWidget::set_open_channels(QStringList strOpenChannels)
{
    nick_list->set_open_channels(strOpenChannels);
    mainWebView->set_open_channels(strOpenChannels);
}

void TabWidget::update_nick_avatar()
{
    nick_list->nicklist_refresh_avatars();
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

void TabWidget::refresh_colors()
{
    // get values
    QSettings settings;
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strJoinFontColor = addslashes(settings.value("font_color_level_1").toString());
    QString strPartFontColor = addslashes(settings.value("font_color_level_2").toString());
    QString strQuitFontColor = addslashes(settings.value("font_color_level_3").toString());
    QString strKickFontColor = addslashes(settings.value("font_color_level_4").toString());
    QString strModeFontColor = addslashes(settings.value("font_color_level_5").toString());
    QString strNoticeFontColor = addslashes(settings.value("font_color_level_6").toString());
    QString strInfoFontColor = addslashes(settings.value("font_color_level_7").toString());
    QString strErrorFontColor = addslashes(settings.value("font_color_level_9").toString());
    QString strChannelFontColor = addslashes(settings.value("channel_font_color").toString());

    // refresh colors
    replace_color("0", strDefaultFontColor);
    replace_color("1", strJoinFontColor);
    replace_color("2", strPartFontColor);
    replace_color("3", strQuitFontColor);
    replace_color("4", strKickFontColor);
    replace_color("5", strModeFontColor);
    replace_color("6", strNoticeFontColor);
    replace_color("7", strInfoFontColor);
    replace_color("8", strDefaultFontColor);
    replace_color("9", strErrorFontColor);
    replace_color("chan", strChannelFontColor);

    // inputline
    inputline->setStyleSheet(QString("color:%1;").arg(strDefaultFontColor));

    // this
    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);

    // mainwebview
    strContentStart = "<html><body style=\"background-color:"+strBackgroundColor+";\">";
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));

    // topic
    QString strTopicContentStart = "<html><body style=\"margin:0;padding:0;font-style:normal;color:"+strDefaultFontColor+";text-align:left;font-family:Verdana;font-weight:normal;font-size:12px;background-color:"+strBackgroundColor+";\">";
    QString strTopicContentEnd = "</body></html>";
    topic->setHtml(strTopicContentStart+strTopicContent+strTopicContentEnd,QUrl(""));
}

// actions

void TabWidget::bold_clicked()
{
    if (bMyBold == true)
    {
        bold->setChecked(false);
        bMyBold = false;
        strMyFontWeight = "";
    }
    else
    {
        bold->setChecked(true);
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
        italic->setChecked(false);
        bMyItalic = false;
        strMyFontStyle = "";
    }
    else
    {
        italic->setChecked(true);
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
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "8px";
}

void TabWidget::size9_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:9px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "9px";
}

void TabWidget::size10_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:10px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "10px";
}

void TabWidget::size11_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:11px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "11px";
}

void TabWidget::size12_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:12px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "12px";
}

void TabWidget::size14_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:14px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "14px";
}

void TabWidget::size18_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:18px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "18px";
}

void TabWidget::size20_triggered()
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:20px");
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = "20px";
}

// color

void TabWidget::color_clicked(int index)
{
    if (index == 0) strCurrentColor = "#000000";
    else if (index == 1) strCurrentColor = "#623c00";
    else if (index == 2) strCurrentColor = "#c86c00";
    else if (index == 3) strCurrentColor = "#ff6500";
    else if (index == 4) strCurrentColor = "#ff0000";
    else if (index == 5) strCurrentColor = "#e40f0f";
    else if (index == 6) strCurrentColor = "#990033";
    else if (index == 7) strCurrentColor = "#8800ab";
    else if (index == 8) strCurrentColor = "#ce00ff";
    else if (index == 9) strCurrentColor = "#0f2ab1";
    else if (index == 10) strCurrentColor = "#3030ce";
    else if (index == 11) strCurrentColor = "#006699";
    else if (index == 12) strCurrentColor = "#1a866e";
    else if (index == 13) strCurrentColor = "#008100";
    else if (index == 14) strCurrentColor = "#959595";
    else strCurrentColor = "#000000";
    inputline->setStyleSheet(QString("color: "+strCurrentColor));
}

// emoticons

void TabWidget::emoticons_clicked()
{
    (new DlgEmoticons(myparent, inputline))->show();
}

// channel settings

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

// moderation

void TabWidget::moderation_clicked()
{
    dlgmoderation->set_active_channel(strName);
    dlgmoderation->show();
}

// clear

void TabWidget::clear_clicked()
{
    strContent.clear();
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
}

// scroll

void TabWidget::scroll_clicked()
{
    if (bScroll == true)
    {
        scroll->setChecked(false);
        bScroll = false;
    }
    else
    {
        scroll->setChecked(true);
        bScroll = true;
    }

    inputline->setFocus();
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

            Commands *pCommands = new Commands(strName, strText);
            strText = pCommands->execute();
            delete pCommands;

            if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
            {
                QStringList strlHelp = strText.split(";");
                for (int i = 0; i < strlHelp.count(); i++)
                    display_msg(strlHelp.at(i), 7);
            }
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

                    if (strCurrentColor != "#000000")
                        strTextDisplay = "%C"+strCurrentColor.right(6)+"%"+strTextDisplay;
                    if ((weight != "") || (font != "verdana"))
                        strTextDisplay = "%F"+weight+":"+font+"%"+strTextDisplay;

                    strTextSend = convert_emots(strTextSend);
                    strTextSend = replace_emots(strTextSend);
                    strTextDisplay = convert_emots(strTextDisplay);
                    strTextDisplay = replace_emots(strTextDisplay);

                    QDateTime dt = QDateTime::currentDateTime();
                    QString strDT = dt.toString("[hh:mm:ss] ");

                    QSettings settings;
                    if (settings.value("disable_logs").toString() == "off")
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

                if (strCurrentColor != "#000000")
                    strText = "%C"+strCurrentColor.right(6)+"%"+strText;
                if ((weight != "") || (font != "verdana"))
                    strText = "%F"+weight+":"+font+"%"+strText;

                strText = convert_emots(strText);
                strText = replace_emots(strText);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                if (bType == true)
                {
                    QSettings settings;
                    if (settings.value("disable_logs").toString() == "off")
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
                    QSettings settings;
                    if (settings.value("disable_logs").toString() == "off")
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

void TabWidget::change_scroll_position()
{
    if (bScroll == true)
        mainWebView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, mainWebView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
    else
        mainWebView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, iScrollBarValue);
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
