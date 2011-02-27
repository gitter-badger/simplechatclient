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

#include <QDateTime>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMessageBox>
#include "dlg_email.h"
#include "network.h"
#include "dlg_channel_settings.h"
#include "simplestatswidget.h"

#ifdef Q_WS_X11
#include <QDebug>
#endif

DlgChannelSettings::DlgChannelSettings(QWidget *parent, Network *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    myparent = parent;
    pNetwork = param1;

    simpleStatsWidget = new SimpleStatsWidget(this);
    simpleStatsWidget->show();
    ui.verticalLayout_stats->addWidget(simpleStatsWidget);

    ui.pushButton_transfer->setIcon(QIcon(":/images/oxygen/16x16/user-group-new.png"));
    ui.pushButton_remove_channel->setIcon(QIcon(":/images/oxygen/16x16/user-group-delete.png"));
    ui.pushButton_set_email->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_website->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_topic->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_bold->setIcon(QIcon(":/images/oxygen/16x16/format-text-bold.png"));
    ui.pushButton_italic->setIcon(QIcon(":/images/oxygen/16x16/format-text-italic.png"));
    ui.pushButton_set_desc->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_password->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_limit->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_op_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_op_del->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_halfop_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_halfop_del->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_ban_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_ban_del->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_invite_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_invite_del->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.tabWidget->setTabText(0, tr("General"));
    ui.tabWidget->setTabText(1, tr("Permissions"));
    ui.tabWidget->setTabText(2, tr("Statistics"));
    ui.toolBox->setItemText(0, tr("Operators"));
    ui.toolBox->setItemText(1, tr("Half-operators"));
    ui.toolBox->setItemText(2, tr("Banned"));
    ui.toolBox->setItemText(3, tr("Invited"));

    ui.label_channel->setText(tr("Channel:"));
    ui.label_owner->setText(tr("Owner:"));
    ui.pushButton_transfer->setText(tr("Transfer"));
    ui.pushButton_remove_channel->setText(tr("Destroy"));
    ui.label_email->setText(tr("Email:"));
    ui.pushButton_set_email->setText(tr("Apply"));
    ui.label_created->setText(tr("Created:"));
    ui.label_datetime->setText(tr("Date/Time"));
    ui.label_website->setText(tr("Website:"));
    ui.pushButton_set_website->setText(tr("Apply"));
    ui.label_topic->setText(tr("Topic:"));
    ui.pushButton_bold->setText("");
    ui.pushButton_italic->setText("");
    ui.pushButton_set_topic->setText(tr("Apply"));
    ui.label_desc->setText(tr("Description:"));
    ui.pushButton_set_desc->setText(tr("Apply"));
    ui.label_status->setText(tr("Status:"));
    ui.radioButton_status_pub->setText(tr("Public"));
    ui.radioButton_status_priv->setText(tr("Private"));
    ui.label_category->setText(tr("Category:"));
    ui.label_guardian->setText(tr("Guardian:"));
    ui.radioButton_guardian_off->setText(tr("Inactive"));
    ui.radioButton_guardian_on->setText(tr("Active"));
    ui.comboBox_guardian_level->setItemText(0, tr("Level 1"));
    ui.comboBox_guardian_level->setItemText(1, tr("Level 2"));
    ui.comboBox_guardian_level->setItemText(2, tr("Level 3"));
    ui.label_password->setText(tr("Password:"));
    ui.pushButton_set_password->setText(tr("Apply"));
    ui.label_limit->setText(tr("Limit:"));
    ui.pushButton_set_limit->setText(tr("Apply"));
    ui.label_moderation->setText(tr("Moderation:"));
    ui.radioButton_moderation_off->setText(tr("Off"));
    ui.radioButton_moderation_on->setText(tr("On"));
    ui.label_auditorium->setText(tr("Auditorium:"));
    ui.radioButton_auditorium_off->setText(tr("Off"));
    ui.radioButton_auditorium_on->setText(tr("On"));
    ui.pushButton_op_add->setText(tr("Add"));
    ui.pushButton_op_del->setText(tr("Remove"));
    ui.pushButton_halfop_add->setText(tr("Add"));
    ui.pushButton_halfop_del->setText(tr("Remove"));
    ui.pushButton_ban_add->setText(tr("Add"));
    ui.pushButton_ban_del->setText(tr("Remove"));
    ui.pushButton_invite_add->setText(tr("Add"));
    ui.pushButton_invite_del->setText(tr("Remove"));
    ui.groupBox_stats->setTitle(tr("Statistics"));
    ui.label_stats_lwords->setText(tr("Average per day spoken words:"));
    ui.label_stats_lfavourites->setText(tr("Channel added in favourites:"));
    ui.label_stats_lexists_days->setText(tr("Channel exists days:"));

    // bold
    ui.pushButton_bold->setCheckable(true);

    // italic
    ui.pushButton_italic->setCheckable(true);

    // font
    QStringList comboBoxFont;
    comboBoxFont << "Arial" << "Times" << "Verdana" << "Tahoma" << "Courier";

    int iComboBoxFont = 0;
    foreach (QString strFont, comboBoxFont)
    {
        ui.comboBox_font->insertItem(iComboBoxFont, strFont);
        iComboBoxFont++;
    }

    // color
    ui.comboBox_color->setToolTip(tr("Font color"));
    ui.comboBox_color->setIconSize(QSize(50,10));

    QStringList comboBoxColors;
    comboBoxColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    int iComboBoxColors = 0;
    foreach (QString strColor, comboBoxColors)
    {
        QPixmap pixmap(50,10);
        pixmap.fill(QColor(strColor));
        ui.comboBox_color->insertItem(iComboBoxColors, pixmap, "");
        iComboBoxColors++;
    }

    QObject::connect(ui.pushButton_transfer, SIGNAL(clicked()), this, SLOT(owner_changed()));
    QObject::connect(ui.pushButton_remove_channel, SIGNAL(clicked()), this, SLOT(remove_channel_clicked()));
    QObject::connect(ui.pushButton_set_email, SIGNAL(clicked()), this, SLOT(email_changed()));
    QObject::connect(ui.pushButton_set_website, SIGNAL(clicked()), this, SLOT(www_changed()));
    QObject::connect(ui.pushButton_set_topic, SIGNAL(clicked()), this, SLOT(topic_changed()));
    QObject::connect(ui.pushButton_set_desc, SIGNAL(clicked()), this, SLOT(desc_changed()));
    QObject::connect(ui.radioButton_status_pub, SIGNAL(clicked()), this, SLOT(status_pub()));
    QObject::connect(ui.radioButton_status_priv, SIGNAL(clicked()), this, SLOT(status_priv()));
    QObject::connect(ui.comboBox_category, SIGNAL(activated(int)), this, SLOT(category_changed(int)));
    QObject::connect(ui.radioButton_guardian_off, SIGNAL(clicked()), this, SLOT(guardian_inactive()));
    QObject::connect(ui.radioButton_guardian_on, SIGNAL(clicked()), this, SLOT(guardian_active()));
    QObject::connect(ui.comboBox_guardian_level, SIGNAL(activated(int)), this, SLOT(guardian_clicked(int)));
    QObject::connect(ui.pushButton_set_password, SIGNAL(clicked()), this, SLOT(password_changed()));
    QObject::connect(ui.pushButton_set_limit, SIGNAL(clicked()), this, SLOT(limit_changed()));
    QObject::connect(ui.radioButton_moderation_off, SIGNAL(clicked()), this, SLOT(moderated_inactive()));
    QObject::connect(ui.radioButton_moderation_on, SIGNAL(clicked()), this, SLOT(moderated_active()));
    QObject::connect(ui.radioButton_auditorium_off, SIGNAL(clicked()), this, SLOT(auditorium_inactive()));
    QObject::connect(ui.radioButton_auditorium_on, SIGNAL(clicked()), this, SLOT(auditorium_active()));

    QObject::connect(ui.pushButton_op_add, SIGNAL(clicked()), this, SLOT(button_op_add()));
    QObject::connect(ui.pushButton_op_del, SIGNAL(clicked()), this, SLOT(button_op_del()));
    QObject::connect(ui.pushButton_halfop_add, SIGNAL(clicked()), this, SLOT(button_halfop_add()));
    QObject::connect(ui.pushButton_halfop_del, SIGNAL(clicked()), this, SLOT(button_halfop_del()));
    QObject::connect(ui.pushButton_ban_add, SIGNAL(clicked()), this, SLOT(button_ban_add()));
    QObject::connect(ui.pushButton_ban_del, SIGNAL(clicked()), this, SLOT(button_ban_del()));
    QObject::connect(ui.pushButton_invite_add, SIGNAL(clicked()), this, SLOT(button_invite_add()));
    QObject::connect(ui.pushButton_invite_del, SIGNAL(clicked()), this, SLOT(button_invite_del()));

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelSettings::set_channel(QString param1)
{
    strChannel = param1;
}

void DlgChannelSettings::set_data(QString strCheckChannel, QMap<QString, QString> mData)
{
    if (strCheckChannel != strChannel) return; // not this channel

    QMapIterator<QString, QString> i(mData);
    while (i.hasNext())
    {
        i.next();

        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "email")
        {
            ui.lineEdit_email->setText(strValue);
        }
        else if (strKey == "createdDate")
        {
            // label created date/time
            QDateTime dt = QDateTime::fromTime_t(strValue.toInt());
            QString strDT = dt.toString("dd/MM/yyyy hh:mm:ss");
            ui.label_datetime->setText(strDT);

            // stats
            int iCreatedDate = strValue.toInt();

            QDateTime cdt = QDateTime::currentDateTime();
            int iCurrentDate = (int)cdt.toTime_t(); // seconds that have passed since 1970

            int iSeconds = iCurrentDate - iCreatedDate;
            int iMinutes = iSeconds / 60;
            int iHours = iMinutes / 60;
            int iDays = iHours / 24;

            ui.label_stats_exists_days->setText(QString::number(iDays));
        }
        else if (strKey == "private")
        {
            if (strValue.toInt() == 1)
                ui.radioButton_status_priv->setChecked(true);
            else if (strValue.toInt() == 0)
                ui.radioButton_status_pub->setChecked(true);
        }
        else if (strKey == "catMajor")
        {
            if (strValue.toInt() == 1) // teen
                ui.comboBox_category->setCurrentIndex(0);
            else if (strValue.toInt() == 2) // towarzyskie
                ui.comboBox_category->setCurrentIndex(1);
            else if (strValue.toInt() == 3) // erotyczne
                ui.comboBox_category->setCurrentIndex(2);
            else if (strValue.toInt() == 4) // tematyczne
                ui.comboBox_category->setCurrentIndex(3);
            else if (strValue.toInt() == 5) // regionalne
                ui.comboBox_category->setCurrentIndex(4);
            else
                ui.comboBox_category->setCurrentIndex(-1);
        }
        else if (strKey == "guardian")
        {
            if (strValue.toInt() == 0)
            {
                ui.radioButton_guardian_off->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(-1);
            }
            else if (strValue.toInt() == 1)
            {
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(0);
            }
            else if (strValue.toInt() == 2)
            {
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(1);
            }
            else if (strValue.toInt() == 3)
            {
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(2);
            }
        }
        else if (strKey == "moderated")
        {
            if (strValue.toInt() == 0)
                ui.radioButton_moderation_off->setChecked(true);
            else if (strValue.toInt() == 1)
                ui.radioButton_moderation_on->setChecked(true);
        }
        else if (strKey == "www")
        {
            ui.lineEdit_website->setText(strValue);
        }
        else if (strKey == "password")
        {
            ui.lineEdit_password->setText(strValue);
        }
        else if (strKey == "limit")
        {
            ui.spinBox_limit->setValue(strValue.toInt());
        }
        else if (strKey == "auditorium")
        {
            if (strValue.toInt() == 0)
                ui.radioButton_auditorium_off->setChecked(true);
            else if (strValue.toInt() == 1)
                ui.radioButton_auditorium_on->setChecked(true);
        }
    }
}

void DlgChannelSettings::set_stats_data(QString strCheckChannel, QMap<QString, QString> mData)
{
    if (strCheckChannel != strChannel) return; // not this channel

    QMapIterator<QString, QString> i(mData);
    while (i.hasNext())
    {
        i.next();

        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "histWords")
        {
            QStringList strlWords = strValue.split(",");
            QList<int> lWords;

            int iWords = 0;
            for (int i = 0; i < strlWords.count(); i++)
            {
                bool ok;
                int iWord = strlWords.at(i).toInt(&ok, 16);

                // add to list
                lWords.append(iWord);

                // add to average words
                iWords += iWord;
            }

            // average words
            iWords /= strlWords.count();
            ui.label_stats_words->setText(QString::number(iWords));

            // simple stats widget
            simpleStatsWidget->set_stats(lWords);
        }
        else if (strKey == "relationsFavourite")
        {
            ui.label_stats_favourites->setText(strValue);
        }
    }
}

void DlgChannelSettings::set_topic(QString strCheckChannel, QString strTopic)
{
    if (strCheckChannel != strChannel) return; // not this channel
    strTopic += " "; // fix convert algorithm

    // convert emoticons
    strTopic.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"), "//\\1");

    // convert font
    while (strTopic.indexOf("%F") != -1)
    {
        int iStartPos = strTopic.indexOf("%F");
        int iEndPos = strTopic.indexOf("%", iStartPos+1);
        int iSpacePos = strTopic.indexOf(" ", iStartPos);

        if (iEndPos != -1)
        {
            if ((iEndPos < iSpacePos) || (iSpacePos == -1))
            {
                iEndPos++;
                QString strFontFull = strTopic.mid(iStartPos, iEndPos-iStartPos);
                QString strFont = strFontFull.mid(2,strFontFull.length()-3);
                strFont = strFont.toLower();

                QString strFontWeight;
                QString strFontName;

                if (strFont.indexOf(":") != -1)
                {
                    strFontWeight = strFont.left(strFont.indexOf(":"));
                    strFontName = strFont.right(strFont.length()-strFont.indexOf(":")-1);
                }
                else
                {
                    QRegExp rx("((b|i)?)((b|i)?)");
                    if (rx.exactMatch(strFont) == true)
                        strFontWeight = strFont;
                }

                if (strFontWeight.isEmpty() == false)
                {
                    for (int fw = 0; fw < strFontWeight.length(); fw++)
                    {
                        if (strFontWeight[fw] == 'b') ui.pushButton_bold->setChecked(true);
                        else if (strFontWeight[fw] == 'i') ui.pushButton_italic->setChecked(true);
                    }
                }

                if ((strFontName.isEmpty() == false) || (strFontWeight.isEmpty() == false))
                {
                    if (strFontName == "arial") ui.comboBox_font->setCurrentIndex(0);
                    else if (strFontName == "times") ui.comboBox_font->setCurrentIndex(1);
                    else if (strFontName == "verdana") ui.comboBox_font->setCurrentIndex(2);
                    else if (strFontName == "tahoma") ui.comboBox_font->setCurrentIndex(3);
                    else if (strFontName == "courier") ui.comboBox_font->setCurrentIndex(4);
                    else ui.comboBox_font->setCurrentIndex(2);

                    strTopic.replace(strFontFull, "");
                }
                else
                    strTopic.insert(iStartPos+1, " "); // fix wrong %F
            }
            else
                strTopic.insert(iStartPos+1, " "); // fix wrong %F
        }
        else
            break;
    }

    // convert color
    QStringList strlFontColors;
    strlFontColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    int iFontColor = 0;
    foreach (QString strFontColor, strlFontColors)
    {
        strFontColor = "%C"+strFontColor.right(6)+"%";

        if (strTopic.contains(strFontColor) == true)
            ui.comboBox_color->setCurrentIndex(iFontColor);

        strTopic.replace(strFontColor, "");
        iFontColor++;
    }

    // default #000000
    if (ui.comboBox_color->currentIndex() == -1)
        ui.comboBox_color->setCurrentIndex(0);

    // default verdana
    if (ui.comboBox_font->currentIndex() == -1)
        ui.comboBox_font->setCurrentIndex(2);

    // set topic
    ui.plainTextEdit_topic->setPlainText(strTopic);
}

void DlgChannelSettings::set_owner(QString strCheckChannel, QString strNick)
{
    if (strCheckChannel != strChannel) return; // not this channel

    ui.label_owner->show();
    ui.label_owner_nick->show();

    ui.label_owner_nick->setText(strNick);
}

void DlgChannelSettings::set_description(QString strCheckChannel, QString strDescription)
{
    if (strCheckChannel != strChannel) return; // not this channel

    ui.plainTextEdit_desc->setPlainText(strDescription);
}

void DlgChannelSettings::add_op(QString strCheckChannel, QString strNick)
{
    if (strCheckChannel != strChannel) return; // not this channel

    if (exist_item(strNick, ui.tableWidget_op) == false)
    {
        ui.tableWidget_op->insertRow(ui.tableWidget_op->rowCount());
        ui.tableWidget_op->setItem(ui.tableWidget_op->rowCount()-1, 0, new QTableWidgetItem(strNick));
    }
}

void DlgChannelSettings::add_halfop(QString strCheckChannel, QString strNick)
{
    if (strCheckChannel != strChannel) return; // not this channel

    if (exist_item(strNick, ui.tableWidget_halfop) == false)
    {
        ui.tableWidget_halfop->insertRow(ui.tableWidget_halfop->rowCount());
        ui.tableWidget_halfop->setItem(ui.tableWidget_halfop->rowCount()-1, 0, new QTableWidgetItem(strNick));
    }
}

void DlgChannelSettings::add_ban(QString strCheckChannel, QString strNick, QString strWho, QString strDT, QString strIPNick)
{
    if (strCheckChannel != strChannel) return; // not this channel

    if (exist_item(strNick, ui.tableWidget_ban) == false)
    {
        ui.tableWidget_ban->insertRow(ui.tableWidget_ban->rowCount());
        ui.tableWidget_ban->setItem(ui.tableWidget_ban->rowCount()-1, 0, new QTableWidgetItem(strNick));
        ui.tableWidget_ban->setItem(ui.tableWidget_ban->rowCount()-1, 1, new QTableWidgetItem(strWho));
        ui.tableWidget_ban->setItem(ui.tableWidget_ban->rowCount()-1, 2, new QTableWidgetItem(strDT));

        if (strIPNick.isEmpty() == true)
        {
            ui.tableWidget_ban->setItem(ui.tableWidget_ban->rowCount()-1, 0, new QTableWidgetItem(strNick));
            ui.tableWidget_ban->setItem(ui.tableWidget_ban->rowCount()-1, 3, new QTableWidgetItem(strIPNick));
        }
        else
        {
            QTableWidgetItem *iIPNick = new QTableWidgetItem(strIPNick);
            iIPNick->setTextColor(QColor("#ff0000")); // set color
            iIPNick->setData(Qt::UserRole, strNick); // set original ban mask
            iIPNick->setToolTip(QString(tr("IP Mask: %1").arg(strNick.remove("*!*@")))); // set tooltip

            ui.tableWidget_ban->setItem(ui.tableWidget_ban->rowCount()-1, 0, iIPNick);
        }
    }
}

void DlgChannelSettings::add_invite(QString strCheckChannel, QString strNick, QString strWho, QString strDT)
{
    if (strCheckChannel != strChannel) return; // not this channel

    if (exist_item(strNick, ui.tableWidget_invite) == false)
    {
        ui.tableWidget_invite->insertRow(ui.tableWidget_invite->rowCount());
        ui.tableWidget_invite->setItem(ui.tableWidget_invite->rowCount()-1, 0, new QTableWidgetItem(strNick));
        ui.tableWidget_invite->setItem(ui.tableWidget_invite->rowCount()-1, 1, new QTableWidgetItem(strWho));
        ui.tableWidget_invite->setItem(ui.tableWidget_invite->rowCount()-1, 2, new QTableWidgetItem(strDT));
    }
}

void DlgChannelSettings::owner_changed()
{
    QString strMsg = "<p style=\"font-weight:bold;\">"+tr("The owner of the channel can be only one!")+"</p><p>"+tr("Enter the nickname of the new owner:")+"</p>";
    bool ok;
    QString strNick = QInputDialog::getText(this, tr("Changing privileges"), strMsg, QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strNick.isEmpty() == false))
        pNetwork->send(QString("CS TRANSFER %1 %2").arg(strChannel).arg(strNick));

    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::remove_channel_clicked()
{
    QString strMsg = "<p style=\"font-weight:bold;\">"+tr("The removal of the channel operation is irreversible!")+"</p>";
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing privileges"), strMsg, QLineEdit::Normal, strChannel, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("CS DROP %1").arg(strText));

    strChannel.clear();
    clear();
    this->hide();
}

void DlgChannelSettings::email_changed()
{
    DlgEmail(myparent, strChannel, ui.lineEdit_email->text()).exec();
}

void DlgChannelSettings::www_changed()
{
    pNetwork->send(QString("CS SET %1 WWW %2").arg(strChannel).arg(ui.lineEdit_website->text()));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::topic_changed()
{
    QString strTopic = ui.plainTextEdit_topic->toPlainText();
    strTopic.replace(QRegExp("(\r|\n)"), "");
    strTopic.replace(QRegExp("(http:|https:)//"), "\\1\\\\"); // fix http https
    strTopic.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
    strTopic.replace(QRegExp("(http:|https:)\\\\\\\\"), "\\1//"); // fix http https

    bool bBold = false;
    bool bItalic = false;
    QString strFontName;
    QString strFontColor;
    QString strFontWeight;

    // bold
    if (ui.pushButton_bold->isChecked() == true) bBold = true;

    // italic
    if (ui.pushButton_italic->isChecked() == true) bItalic = true;

    // font name
    strFontName = ui.comboBox_font->currentText().toLower();

    // font color
    QStringList strlFontColors;
    strlFontColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    if (ui.comboBox_color->currentIndex() != -1)
        strFontColor = strlFontColors.at(ui.comboBox_color->currentIndex());

    // set topic
    if (bBold == true) strFontWeight += "b";
    if (bItalic == true) strFontWeight += "i";

    if (strFontName == "verdana")
        strFontName = "";
    if ((strFontColor != "#000000") && (strFontColor.isEmpty() == false))
        strTopic = "%C"+strFontColor.right(6)+"%"+strTopic;
    if (strFontName.isEmpty() == false)
        strFontName = ":"+strFontName;
    if ((strFontWeight.isEmpty() == false) || (strFontName.isEmpty() == false))
        strTopic = "%F"+strFontWeight+strFontName+"%"+strTopic;

    pNetwork->send(QString("CS SET %1 TOPIC %2").arg(strChannel).arg(strTopic));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::desc_changed()
{
    pNetwork->send(QString("CS SET %1 LONGDESC %2").arg(strChannel).arg(ui.plainTextEdit_desc->toPlainText().replace(QRegExp("(\r|\n)"), "")));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::status_pub()
{
    pNetwork->send(QString("CS SET %1 PRIVATE OFF").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::status_priv()
{
    pNetwork->send(QString("CS SET %1 PRIVATE ON").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::category_changed(int index)
{
    pNetwork->send(QString("CS SET %1 CATMAJOR %2").arg(strChannel).arg(index+1));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::guardian_inactive()
{
    pNetwork->send(QString("CS SET %1 GUARDIAN 0").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::guardian_active()
{
    if (ui.comboBox_guardian_level->currentIndex() != -1)
        pNetwork->send(QString("CS SET %1 GUARDIAN %2").arg(strChannel).arg(ui.comboBox_guardian_level->currentIndex()));
    else
        pNetwork->send(QString("CS SET %1 GUARDIAN 1").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::guardian_clicked(int iLevel)
{
    if (ui.radioButton_guardian_on->isChecked() == true)
        pNetwork->send(QString("CS SET %1 GUARDIAN %2").arg(strChannel).arg(iLevel+1));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::password_changed()
{
    pNetwork->send(QString("CS SET %1 PASSWORD %2").arg(strChannel).arg(ui.lineEdit_password->text()));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::limit_changed()
{
    pNetwork->send(QString("CS SET %1 LIMIT %2").arg(strChannel).arg(ui.spinBox_limit->value()));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::moderated_inactive()
{
    pNetwork->send(QString("CS SET %1 MODERATED OFF").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::moderated_active()
{
    pNetwork->send(QString("CS SET %1 MODERATED ON").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::auditorium_inactive()
{
    pNetwork->send(QString("CS SET %1 AUDITORIUM OFF").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::auditorium_active()
{
    pNetwork->send(QString("CS SET %1 AUDITORIUM ON").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_op_add()
{
    bool ok;
    QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Add super-operator:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strNick.isEmpty() == false))
        pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));

    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_op_del()
{
    if (ui.tableWidget_op->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget_op->selectedItems().at(0)->text();
        pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        bool ok;
        QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Remove the super-operator:"), QLineEdit::Normal, QString::null, &ok);

        if ((ok == true) && (strNick.isEmpty() == false))
            pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));

        clear();
        ui.label_channel_name->setText(strChannel);
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
    }
}

void DlgChannelSettings::button_halfop_add()
{
    bool ok;
    QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Add operator:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strNick.isEmpty() == false))
        pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));

    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_halfop_del()
{
    if (ui.tableWidget_halfop->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget_halfop->selectedItems().at(0)->text();
        pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        bool ok;
        QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Remove operator:"), QLineEdit::Normal, QString::null, &ok);

        if ((ok == true) && (strNick.isEmpty() == false))
            pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));

        clear();
        ui.label_channel_name->setText(strChannel);
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
    }
}

void DlgChannelSettings::button_ban_add()
{
    bool ok;
    QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Add ban:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strNick.isEmpty() == false))
    {
        if (strNick.contains("*") == true)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(tr("This ban will ban large numbers of people. Are you sure you want to use this ban?"));
            int iResult = msgBox.exec();

            if (iResult == QMessageBox::Ok)
                pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
        }
        else
            pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
    }

    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_ban_del()
{
    if (ui.tableWidget_ban->selectedItems().isEmpty() == false)
    {
        bool bIPBan = false;
        if (ui.tableWidget_ban->selectedItems().at(0)->data(Qt::UserRole).isNull() == false)
            bIPBan = true;

        QString strRemoveNick = ui.tableWidget_ban->selectedItems().at(0)->text();
        if (bIPBan == true)
            pNetwork->send(QString("CS BANIP %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        else
            pNetwork->send(QString("CS BAN %1 DEL %2").arg(strChannel).arg(strRemoveNick));

        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        bool ok;
        QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Remove ban:"), QLineEdit::Normal, QString::null, &ok);

        if ((ok == true) && (strNick.isEmpty() == false))
            pNetwork->send(QString("CS BAN %1 DEL %2").arg(strChannel).arg(strNick));

        clear();
        ui.label_channel_name->setText(strChannel);
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
    }
}

void DlgChannelSettings::button_invite_add()
{
    bool ok;
    QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Add invitation:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strNick.isEmpty() == false))
        pNetwork->send(QString("CS INVITE %1 ADD %2").arg(strChannel).arg(strNick));

    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_invite_del()
{
    if (ui.tableWidget_invite->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget_invite->selectedItems().at(0)->text();
        pNetwork->send(QString("CS INVITE %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        bool ok;
        QString strNick = QInputDialog::getText(this, tr("Changing privileges"), tr("Delete invitation:"), QLineEdit::Normal, QString::null, &ok);

        if ((ok == true) && (strNick.isEmpty() == false))
            pNetwork->send(QString("CS INVITE %1 DEL %2").arg(strChannel).arg(strNick));

        clear();
        ui.label_channel_name->setText(strChannel);
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
    }
}

void DlgChannelSettings::button_ok()
{
    strChannel.clear();
    clear();
    this->hide();
}

void DlgChannelSettings::button_cancel()
{
    strChannel.clear();
    clear();
    this->hide();
}

bool DlgChannelSettings::exist_item(QString strItem, QTableWidget *list)
{
    for (int i = 0; i < list->rowCount(); i++)
    {
        if (list->item(0, i)->text() == strItem)
            return true;
    }
    return false;
}

void DlgChannelSettings::clear()
{
    // switch tab
    ui.tabWidget->setCurrentIndex(0);

    // clear
    ui.tableWidget_op->clear();
    ui.tableWidget_halfop->clear();
    ui.tableWidget_ban->clear();
    ui.tableWidget_invite->clear();

    ui.tableWidget_op->setRowCount(0);
    ui.tableWidget_halfop->setRowCount(0);
    ui.tableWidget_ban->setRowCount(0);
    ui.tableWidget_invite->setRowCount(0);

    QStringList strlLabels;
    strlLabels << tr("Nick");

    QStringList strlLabels2;
    strlLabels2 << tr("Nick") << tr("Created by") << tr("Date/Time");

    ui.tableWidget_op->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_halfop->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_ban->setHorizontalHeaderLabels(strlLabels2);
    ui.tableWidget_invite->setHorizontalHeaderLabels(strlLabels2);

    // prevents crash!
    ui.tableWidget_op->setSortingEnabled(false);
    ui.tableWidget_halfop->setSortingEnabled(false);
    ui.tableWidget_ban->setSortingEnabled(false);
    ui.tableWidget_invite->setSortingEnabled(false);

    ui.lineEdit_email->clear();
    ui.lineEdit_website->clear();
    ui.lineEdit_password->clear();
    ui.plainTextEdit_topic->clear();
    ui.pushButton_bold->setChecked(false);
    ui.pushButton_italic->setChecked(false);
    ui.comboBox_font->setCurrentIndex(-1);
    ui.comboBox_color->setCurrentIndex(-1);
    ui.plainTextEdit_desc->clear();
    ui.label_channel_name->clear();
    ui.label_owner_nick->clear();
    ui.label_datetime->clear();
    ui.spinBox_limit->setValue(0);
    ui.comboBox_category->setCurrentIndex(-1);
    ui.comboBox_guardian_level->setCurrentIndex(-1);
    ui.radioButton_status_pub->setChecked(false);
    ui.radioButton_status_priv->setChecked(false);
    ui.radioButton_guardian_off->setChecked(false);
    ui.radioButton_guardian_on->setChecked(false);
    ui.radioButton_moderation_off->setChecked(false);
    ui.radioButton_moderation_on->setChecked(false);
    ui.radioButton_auditorium_off->setChecked(false);
    ui.radioButton_auditorium_on->setChecked(false);
    ui.label_stats_favourites->setText("-");
    ui.label_stats_words->setText("-");
    ui.label_stats_exists_days->setText("-");
}

void DlgChannelSettings::showEvent(QShowEvent *event)
{
    event->accept();
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    // clear
    clear();
    simpleStatsWidget->clear_stats();
    ui.label_owner->hide();
    ui.label_owner_nick->hide();
    ui.label_channel_name->setText(strChannel);

    // get data
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
    pNetwork->send(QString("RS INFO %1").arg(strChannel));
}

void DlgChannelSettings::hideEvent(QHideEvent *event)
{
    event->accept();

    clear();
}

void DlgChannelSettings::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
