/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include <QComboBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QMenu>
#include <QToolButton>
#include "core.h"
#include "commands.h"
#include "config.h"
#include "channel_settings.h"
#include "emoticons.h"
#include "moderation.h"
#include "inputline_widget.h"
#include "replace.h"
#include "tool_widget.h"

ToolWidget::ToolWidget(QWidget *parent) : QWidget(parent), strCurrentColor("#000000")
{
    showFontButtons = new QToolButton(this);
    showFontButtons->setIcon(QIcon(":/images/oxygen/16x16/format-text-color.png"));
    showFontButtons->setToolTip(tr("Show/Hide font buttons"));
    showFontButtons->show();

    separator1 = new QFrame(this);
    separator1->setFrameShape(QFrame::VLine);
    separator1->setFrameShadow(QFrame::Sunken);
    separator1->show();

    bold = new QToolButton(this);
    bold->setIcon(QIcon(":/images/oxygen/16x16/format-text-bold.png"));
    bold->setToolTip(tr("Bold"));
    bold->setCheckable(true);
    bold->setAutoRaise(true);
    bold->show();
    bMyBold = false;

    italic = new QToolButton(this);
    italic->setIcon(QIcon(":/images/oxygen/16x16/format-text-italic.png"));
    italic->setToolTip(tr("Italic"));
    italic->setCheckable(true);
    italic->setAutoRaise(true);
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

    fontfamily = new QToolButton(this);
    fontfamily->setToolTip(tr("Font family"));
    fontfamily->setFont(QFont("Verdana", -1, -1, false));
    fontfamily->setText("Verdana");
    fontfamily->setPopupMode(QToolButton::MenuButtonPopup);
    fontfamily->setMenu(fontMenu);
    fontfamily->show();

    color = new QComboBox(this);
    color->setToolTip(tr("Font color"));
    color->setIconSize(QSize(20,10));

    QStringList comboBoxColors;
    comboBoxColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    foreach (QString strColor, comboBoxColors)
    {
        QPixmap pixmap(20,10);
        pixmap.fill(QColor(strColor));
        color->addItem(QIcon(pixmap), "");
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
    size16Act = new QAction("16", this);
    size16Act->setFont(QFont("Verdana", 16, -1, false));
    size18Act = new QAction("18", this);
    size18Act->setFont(QFont("Verdana", 18, -1, false));
    size20Act = new QAction("20", this);
    size20Act->setFont(QFont("Verdana", 20, -1, false));
    size24Act = new QAction("24", this);
    size24Act->setFont(QFont("Verdana", 24, -1, false));

    sizeMenu = new QMenu(this);
    sizeMenu->addAction(size8Act);
    sizeMenu->addAction(size9Act);
    sizeMenu->addAction(size10Act);
    sizeMenu->addAction(size11Act);
    sizeMenu->addAction(size12Act);
    sizeMenu->addAction(size14Act);
    sizeMenu->addAction(size16Act);
    sizeMenu->addAction(size18Act);
    sizeMenu->addAction(size20Act);
    sizeMenu->addAction(size24Act);

    size = new QToolButton(this);
    size->setIcon(QIcon(":/images/oxygen/16x16/format-font-size-more.png"));
    size->setToolTip(tr("Font size"));
    size->setFont(QFont("Times New Roman", -1, -1, false));
    size->setPopupMode(QToolButton::MenuButtonPopup);
    size->setMenu(sizeMenu);
    size->show();

    separator2 = new QFrame(this);
    separator2->setFrameShape(QFrame::VLine);
    separator2->setFrameShadow(QFrame::Sunken);
    separator2->show();

    emoticons = new QToolButton(this);
    emoticons->setIcon(QIcon(":/images/oxygen/16x16/face-smile.png"));
    emoticons->setToolTip(tr("Emoticons"));
    emoticons->show();

    channel_settings = new QToolButton(this);
    channel_settings->setIcon(QIcon(":/images/oxygen/16x16/configure.png"));
    channel_settings->setToolTip(tr("Channel settings"));
    channel_settings->show();

    moderation = new QToolButton(this);
    moderation->setIcon(QIcon(":/images/oxygen/16x16/layer-visible-on.png"));
    moderation->setToolTip(tr("Moderation"));
    moderation->show();

    nickLabel = new QLabel(parent);
    nickLabel->setText(tr("(Unregistered)"));
    nickLabel->setStyleSheet("font-weight:bold;margin-left:1px;");
    nickLabel->show();

    pInputLine = new InputLineWidget(this);
    pInputLine->setMinimumWidth(350);
    pInputLine->show();

    sendButton = new QToolButton(this);
    sendButton->setIcon(QIcon(":/images/oxygen/16x16/go-jump-locationbar.png"));
    sendButton->setToolTip(tr("Send"));
    sendButton->show();

    moderSendButton = new QToolButton(this);
    moderSendButton->setIcon(QIcon(":/images/oxygen/16x16/meeting-observer.png"));
    moderSendButton->setToolTip(tr("Send to moderators"));
    moderSendButton->show();

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setMargin(0);
    topLayout->setAlignment(Qt::AlignLeft);
    topLayout->addWidget(showFontButtons);
    topLayout->addWidget(separator1);
    topLayout->addWidget(bold);
    topLayout->addWidget(italic);
    topLayout->addWidget(fontfamily);
    topLayout->addWidget(color);
    topLayout->addWidget(size);
    topLayout->addWidget(separator2);
    topLayout->addWidget(emoticons);
    topLayout->addWidget(channel_settings);
    topLayout->addWidget(moderation);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->setAlignment(Qt::AlignLeft);
    bottomLayout->addWidget(nickLabel);
    bottomLayout->addWidget(pInputLine);
    bottomLayout->addWidget(sendButton);
    bottomLayout->addWidget(moderSendButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    // set default values
    setDefaultValues();

    // focus
    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(pInputLine);

    // signals
    QObject::connect(showFontButtons, SIGNAL(clicked()), this, SLOT(showFontButtonsClicked()));

    QObject::connect(bold, SIGNAL(clicked()), this, SLOT(boldClicked()));
    QObject::connect(italic, SIGNAL(clicked()), this, SLOT(italicClicked()));

    QObject::connect(arialAct, SIGNAL(triggered()), this, SLOT(arialTriggered()));
    QObject::connect(timesAct, SIGNAL(triggered()), this, SLOT(timesTriggered()));
    QObject::connect(verdanaAct, SIGNAL(triggered()), this, SLOT(verdanaTriggered()));
    QObject::connect(tahomaAct, SIGNAL(triggered()), this, SLOT(tahomaTriggered()));
    QObject::connect(courierAct, SIGNAL(triggered()), this, SLOT(courierTriggered()));

    QObject::connect(size8Act, SIGNAL(triggered()), this, SLOT(size8Triggered()));
    QObject::connect(size9Act, SIGNAL(triggered()), this, SLOT(size9Triggered()));
    QObject::connect(size10Act, SIGNAL(triggered()), this, SLOT(size10Triggered()));
    QObject::connect(size11Act, SIGNAL(triggered()), this, SLOT(size11Triggered()));
    QObject::connect(size12Act, SIGNAL(triggered()), this, SLOT(size12Triggered()));
    QObject::connect(size14Act, SIGNAL(triggered()), this, SLOT(size14Triggered()));
    QObject::connect(size16Act, SIGNAL(triggered()), this, SLOT(size16Triggered()));
    QObject::connect(size18Act, SIGNAL(triggered()), this, SLOT(size18Triggered()));
    QObject::connect(size20Act, SIGNAL(triggered()), this, SLOT(size20Triggered()));
    QObject::connect(size24Act, SIGNAL(triggered()), this, SLOT(size24Triggered()));

    QObject::connect(color, SIGNAL(activated(int)), this, SLOT(colorClicked(int)));
    QObject::connect(emoticons, SIGNAL(clicked()), this, SLOT(emoticonsClicked()));
    QObject::connect(channel_settings, SIGNAL(clicked()), this, SLOT(channelSettingsClicked()));
    QObject::connect(moderation, SIGNAL(clicked()), this, SLOT(moderationClicked()));

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputlineReturnPressed()));
    QObject::connect(pInputLine, SIGNAL(returnPressed()), this, SLOT(inputlineReturnPressed()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moderButtonClicked()));
    QObject::connect(pInputLine, SIGNAL(ctrlTabPressed()), this, SLOT(slotCtrlTabPressed()));
    QObject::connect(pInputLine, SIGNAL(ctrlShiftTabPressed()), this, SLOT(slotCtrlShiftTabPressed()));
}

ToolWidget::~ToolWidget()
{
    delete pInputLine;
}

void ToolWidget::setDefaultValues()
{
    // font buttons
    bShowFontButtons = false;
    separator1->hide();
    bold->hide();
    italic->hide();
    fontfamily->hide();
    color->hide();
    size->hide();
    separator2->hide();

    // set default bold
    if (Core::instance()->settings.value("my_bold") == "on")
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
    if (Core::instance()->settings.value("my_italic") == "on")
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

    int iWeight = (bMyBold ? 75 : 50);

    // set default font
    QString strMyFont = Core::instance()->settings.value("my_font");
    fontfamily->setText(strMyFont);
    strMyFontFamily = strMyFont;

    // set default color
    QString strMyColor = Core::instance()->settings.value("my_color");
    strCurrentColor = strMyColor;

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

    // refresh input widget
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));

    // refresh input line text color
    if ((Core::instance()->settings["background_color"] == "#000000") && (strCurrentColor == "#000000"))
        pInputLine->setStyleSheet(QString("QLineEdit { color:#ffffff; }"));
    else
        pInputLine->setStyleSheet(QString("QLineEdit { color:%1; }").arg(strCurrentColor));

    // moderation default hidden
    setModeration(false);
}

void ToolWidget::setModeration(bool bEnable)
{
    if (bEnable)
    {
        moderation->show();
        moderSendButton->show();
    }
    else
    {
        moderation->hide();
        moderSendButton->hide();
    }
}

void ToolWidget::setChannelSettings(bool bEnable)
{
    if (bEnable)
        channel_settings->show();
    else
        channel_settings->hide();
}

void ToolWidget::updateNick(QString strNick)
{
    nickLabel->setText(strNick);
}

void ToolWidget::showFontButtonsClicked()
{
    if (bShowFontButtons)
    {
        bShowFontButtons = false;
        separator1->hide();
        bold->hide();
        italic->hide();
        fontfamily->hide();
        color->hide();
        size->hide();
        separator2->hide();
    }
    else
    {
        bShowFontButtons = true;
        separator1->show();
        bold->show();
        italic->show();
        fontfamily->show();
        color->show();
        size->show();
        separator2->show();
    }
}

void ToolWidget::boldClicked()
{
    if (bMyBold)
    {
        bold->setChecked(false);
        bMyBold = false;
        strMyFontWeight = "";
        QString strValue = "off";
        Core::instance()->settings["my_bold"] = strValue;

        Config *pConfig = new Config();
        pConfig->setValue("my_bold", strValue);
        delete pConfig;
    }
    else
    {
        bold->setChecked(true);
        bMyBold = true;
        strMyFontWeight = "bold";
        QString strValue = "on";
        Core::instance()->settings["my_bold"] = strValue;

        Config *pConfig = new Config();
        pConfig->setValue("my_bold", strValue);
        delete pConfig;
    }

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::italicClicked()
{
    if (bMyItalic)
    {
        italic->setChecked(false);
        bMyItalic = false;
        strMyFontStyle = "";
        QString strValue = "off";
        Core::instance()->settings["my_italic"] = strValue;

        Config *pConfig = new Config();
        pConfig->setValue("my_italic", strValue);
        delete pConfig;
    }
    else
    {
        italic->setChecked(true);
        bMyItalic = true;
        strMyFontStyle = "italic";
        QString strValue = "on";
        Core::instance()->settings["my_italic"] = "on";

        Config *pConfig = new Config();
        pConfig->setValue("my_italic", strValue);
        delete pConfig;
    }

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::arialTriggered()
{
    fontfamily->setText("Arial");
    strMyFontFamily = "Arial";
    Core::instance()->settings["my_font"] = strMyFontFamily;
    Config *pConfig = new Config();
    pConfig->setValue("my_font", strMyFontFamily);
    delete pConfig;

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::timesTriggered()
{
    fontfamily->setText("Times");
    strMyFontFamily = "Times";
    Core::instance()->settings["my_font"] = strMyFontFamily;
    Config *pConfig = new Config();
    pConfig->setValue("my_font", strMyFontFamily);
    delete pConfig;

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::verdanaTriggered()
{
    fontfamily->setText("Verdana");
    strMyFontFamily = "Verdana";
    Core::instance()->settings["my_font"] = strMyFontFamily;
    Config *pConfig = new Config();
    pConfig->setValue("my_font", strMyFontFamily);
    delete pConfig;

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::tahomaTriggered()
{
    fontfamily->setText("Tahoma");
    strMyFontFamily = "Tahoma";
    Core::instance()->settings["my_font"] = strMyFontFamily;
    Config *pConfig = new Config();
    pConfig->setValue("my_font", strMyFontFamily);
    delete pConfig;

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::courierTriggered()
{
    fontfamily->setText("Courier");
    strMyFontFamily = "Courier";
    Core::instance()->settings["my_font"] = strMyFontFamily;
    Config *pConfig = new Config();
    pConfig->setValue("my_font", strMyFontFamily);
    delete pConfig;

    int iWeight = (bMyBold ? 75 : 50);
    pInputLine->setFont(QFont(strMyFontFamily, -1, iWeight, bMyItalic));
}

void ToolWidget::size8Triggered()
{
    QString strValue = "8px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size9Triggered()
{
    QString strValue = "9px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size10Triggered()
{
    QString strValue = "10px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size11Triggered()
{
    QString strValue = "11px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size12Triggered()
{
    QString strValue = "12px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size14Triggered()
{
    QString strValue = "14px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size16Triggered()
{
    QString strValue = "16px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size18Triggered()
{
    QString strValue = "18px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size20Triggered()
{
    QString strValue = "20px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::size24Triggered()
{
    QString strValue = "24px";

    Core::instance()->settings["font_size"] = strValue;
    Config *pConfig = new Config();
    pConfig->setValue("font_size", strValue);
    delete pConfig;

    Core::instance()->refreshCSS();
}

void ToolWidget::colorClicked(int index)
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

    Core::instance()->settings["my_color"] = strCurrentColor;
    Config *pConfig = new Config();
    pConfig->setValue("my_color", strCurrentColor);
    delete pConfig;

    // refresh input line text color
    if ((Core::instance()->settings["background_color"] == "#000000") && (strCurrentColor == "#000000"))
        pInputLine->setStyleSheet(QString("QLineEdit { color:#ffffff; }"));
    else
        pInputLine->setStyleSheet(QString("QLineEdit { color:%1; }").arg(strCurrentColor));
}

void ToolWidget::emoticonsClicked()
{
    DlgEmoticons(Core::instance()->sccWindow(), pInputLine).exec();
}

void ToolWidget::channelSettingsClicked()
{
    if (Core::instance()->pNetwork->isConnected())
        DlgChannelSettings(this, Core::instance()->getCurrentChannelName()).exec();
}

void ToolWidget::moderationClicked()
{
    QString strChannel = Core::instance()->getCurrentChannelName();

    DlgModeration(this, strChannel).exec();
}

void ToolWidget::inputlineReturnPressed()
{
    // update last active
    QDateTime cdt = QDateTime::currentDateTime();
    int t = (int)cdt.toTime_t(); // seconds that have passed since 1970
    Core::instance()->settings["last_active"] = QString::number(t);

    // disable away
    bool bAway = Core::instance()->settings.value("away") == "on" ? true : false;
    if (bAway)
        Core::instance()->pNetwork->send("AWAY :");

    // text
    QString strText = pInputLine->text().trimmed();
    pasteMultiLine(strText, false);
    pInputLine->clear();
}

void ToolWidget::moderButtonClicked()
{
    // update last active
    QDateTime cdt = QDateTime::currentDateTime();
    int t = (int)cdt.toTime_t(); // seconds that have passed since 1970
    Core::instance()->settings["last_active"] = QString::number(t);

    // disable away
    bool bAway = Core::instance()->settings.value("away") == "on" ? true : false;
    if (bAway)
        Core::instance()->pNetwork->send("AWAY :");

    // text
    QString strText = pInputLine->text().trimmed();
    pasteMultiLine(strText, true);
    pInputLine->clear();
}

void ToolWidget::slotCtrlTabPressed()
{
    emit ctrlTabPressed();
}

void ToolWidget::slotCtrlShiftTabPressed()
{
    emit ctrlShiftTabPressed();
}

void ToolWidget::pasteMultiLine(QString strText, bool bModeration)
{
    QStringList list = strText.split(QRegExp("(\n|\r)"));
    int len = 400;

    for (int i = 0; i < list.size(); i++)
    {
        QString line = list.at(i);
        if (line.size() > len)
        {
            while (line.size() > len)
            {
                QString short_line = line.left(len);
                sendMessage(short_line, bModeration);
                line.remove(0, len);
            }
        }
        if ((line.size() < len) && (line.size() != 0))
            sendMessage(line, bModeration);
    }
}

void ToolWidget::sendMessage(QString strText, bool bModeration)
{
    if (strText.isEmpty()) return; // empty text!
    QString strChannel = Core::instance()->getCurrentChannelName();

    QString strTextOriginal = strText;

    QString strMe = Core::instance()->settings.value("nick");
    QString strCurrentColor = Core::instance()->settings.value("my_color");
    QString strFontFamily = Core::instance()->settings.value("my_font");
    bool bMyBold = Core::instance()->settings.value("my_bold") == "on" ? true : false;
    bool bMyItalic = Core::instance()->settings.value("my_italic") == "on" ? true : false;

    // if command
    if ((strText[0] == '/') && (strText[1] != '/'))
    {
        if (strText[0] == '/')
            strText = strText.right(strText.length()-1);
        strTextOriginal = strText;
        QStringList strTextList = strText.split(" ");

        Commands *pCommands = new Commands(strChannel, strText);
        strText = pCommands->execute();
        delete pCommands;

        // help
        if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
        {
            QStringList lHelp = strText.split(";");
            for (int i = 0; i < lHelp.size(); i++)
            {
                QString strDisplay = lHelp.at(i);
                emit showMessage(strChannel, strDisplay, InfoMessage);
            }
        }
        else if ((strTextList[0] == "mp3") || (strTextList[0] == "winamp"))
        {
            if (strChannel == "Status") return; // return if status

            QString weight;
            QString font = strFontFamily.toLower();

            if (bMyBold) weight += "b";
            if (bMyItalic) weight += "i";

            if (font == "verdana")
                font = "";
            if ((strCurrentColor != "#000000") && (!strCurrentColor.isEmpty()))
                strText = "%C"+strCurrentColor.right(6)+"%"+strText;
            if (!font.isEmpty())
                font = ":"+font;
            if ((!weight.isEmpty()) || (!font.isEmpty()))
                strText = "%F"+weight+font+"%"+strText;

            Core::instance()->pNetwork->send(QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText));
            emit showMessage(strChannel, strText, DefaultMessage, QString::null, strMe);
        }
        // me
        else if (strTextList[0] == "me")
        {
            if (strChannel == "Status") return; // return if status

            if (strTextOriginal.length() > 3)
            {
                QString strTextSend = strText;
                QString strTextDisplay = strTextOriginal.right(strTextOriginal.length()-3);

                QString weight;
                QString font = strFontFamily.toLower();

                if (bMyBold) weight += "b";
                if (bMyItalic) weight += "i";

                if (font == "verdana")
                    font = "";
                if ((strCurrentColor != "#000000") && (!strCurrentColor.isEmpty()))
                    strTextDisplay = "%C"+strCurrentColor.right(6)+"%"+strTextDisplay;
                if (!font.isEmpty())
                    font = ":"+font;
                if ((!weight.isEmpty()) || (!font.isEmpty()))
                    strTextDisplay = "%F"+weight+font+"%"+strTextDisplay;

                Replace *pReplace = new Replace();
                pReplace->convertAndReplaceEmots(strTextSend);
                pReplace->convertAndReplaceEmots(strTextDisplay);
                delete pReplace;

                Core::instance()->pNetwork->send(strTextSend);
                QString strDisplay = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01")));
                emit showMessage(strChannel, strDisplay, MeMessage, QString::null, strMe);
            }
        }
        // other command
        else
        {
            if (strText.length() > 0)
                Core::instance()->pNetwork->send(strText);
        }
    }
    else
    {
        if (strChannel == "Status") return; // return if status

        QString weight;
        QString font = strFontFamily.toLower();

        if (bMyBold) weight += "b";
        if (bMyItalic) weight += "i";

        if (font == "verdana")
            font = "";
        if ((strCurrentColor != "#000000") && (!strCurrentColor.isEmpty()))
            strText = "%C"+strCurrentColor.right(6)+"%"+strText;
        if (!font.isEmpty())
            font = ":"+font;
        if ((!weight.isEmpty()) || (!font.isEmpty()))
            strText = "%F"+weight+font+"%"+strText;

        Replace *pReplace = new Replace();
        pReplace->convertAndReplaceEmots(strText);
        delete pReplace;

        // standard text
        if (!bModeration)
        {
            strText = QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText);
            Core::instance()->pNetwork->send(strText);
            QString strDisplay = strText.right(strText.length()-10-strChannel.length());
            emit showMessage(strChannel, strDisplay, DefaultMessage, QString::null, strMe);
        }
        // moder notice
        else if (bModeration)
        {
            strText = QString("MODERNOTICE %1 :%2").arg(strChannel).arg(strText);
            Core::instance()->pNetwork->send(strText);
            QString strDisplay = strText.right(strText.length()-14-strChannel.length());
            emit showMessage(strChannel, strDisplay, ModerNoticeMessage, QString::null, strMe);
        }
    }
}
