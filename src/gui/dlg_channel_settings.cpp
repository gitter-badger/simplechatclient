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
#include <QSettings>
#include "core.h"
#include "dlg_email.h"
#include "network.h"
#include "dlg_channel_settings.h"
#include "simplestatswidget.h"

DlgChannelSettings::DlgChannelSettings(QWidget *parent, Network *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    pNetwork = param1;

    pSimpleStatsWidget = new SimpleStatsWidget(this);
    pSimpleStatsWidget->show();
    ui.verticalLayout_stats->addWidget(pSimpleStatsWidget);

    create_gui();
    set_default_values();
    create_signals();
}

void DlgChannelSettings::create_gui()
{
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
    ui.pushButton_permission_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_permission_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.tabWidget->setTabText(0, tr("Summary"));
    ui.tabWidget->setTabText(1, tr("General"));
    ui.tabWidget->setTabText(2, tr("Advanced"));
    ui.tabWidget->setTabText(3, tr("Permissions"));
    ui.tabWidget->setTabText(4, tr("Statistics"));
    ui.listWidget_permissions->insertItem(0, tr("Operators"));
    ui.listWidget_permissions->insertItem(1, tr("Half-operators"));
    ui.listWidget_permissions->insertItem(2, tr("Banned"));
    ui.listWidget_permissions->insertItem(3, tr("Invited"));

    // summary
    ui.label_created1->setText(tr("Created:"));
    ui.label_website1->setText(tr("Website:"));
    ui.label_topic1->setText(tr("Topic:"));
    ui.label_description1->setText(tr("Description:"));
    ui.label_status1->setText(tr("Status:"));
    ui.label_category1->setText(tr("Category:"));
    ui.label_type1->setText(tr("Type:"));
    ui.label_guardian1->setText(tr("Guardian:"));
    ui.label_moderation1->setText(tr("Moderation:"));

    // general
    ui.label_owner->setText(tr("Owner:"));
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
    ui.comboBox_category->setItemText(0, tr("Teen"));
    ui.comboBox_category->setItemText(1, tr("Common"));
    ui.comboBox_category->setItemText(2, tr("Erotic"));
    ui.comboBox_category->setItemText(3, tr("Thematic"));
    ui.comboBox_category->setItemText(4, tr("Regional"));
    ui.label_guardian->setText(tr("Guardian:"));
    ui.radioButton_guardian_off->setText(tr("Inactive"));
    ui.radioButton_guardian_on->setText(tr("Active"));
    ui.comboBox_guardian_level->setItemText(0, tr("Level 1"));
    ui.comboBox_guardian_level->setItemText(1, tr("Level 2"));
    ui.comboBox_guardian_level->setItemText(2, tr("Level 3"));
    ui.label_moderation->setText(tr("Moderation:"));
    ui.radioButton_moderation_off->setText(tr("Off"));
    ui.radioButton_moderation_on->setText(tr("On"));

    // advanced
    ui.label_channel->setText(tr("Channel:"));
    ui.pushButton_transfer->setText(tr("Transfer"));
    ui.pushButton_remove_channel->setText(tr("Destroy"));
    ui.label_email->setText(tr("Email:"));
    ui.pushButton_set_email->setText(tr("Apply"));
    ui.label_password->setText(tr("Password:"));
    ui.pushButton_set_password->setText(tr("Apply"));
    ui.label_limit->setText(tr("Limit:"));
    ui.pushButton_set_limit->setText(tr("Apply"));
    ui.label_auditorium->setText(tr("Auditorium:"));
    ui.radioButton_auditorium_off->setText(tr("Off"));
    ui.radioButton_auditorium_on->setText(tr("On"));

    // permissions
    ui.label_permission_op->setText(tr("Operators"));
    ui.label_permission_halfop->setText(tr("Half-operators"));
    ui.label_permission_ban->setText(tr("Banned"));
    ui.label_permission_invite->setText(tr("Invited"));
    ui.pushButton_permission_add->setText(tr("Add"));
    ui.pushButton_permission_remove->setText(tr("Remove"));

    // statistics
    ui.groupBox_stats->setTitle(tr("Statistics"));
    ui.label_stats_lwords->setText(tr("Average per day spoken words:"));
    ui.label_stats_lfavourites->setText(tr("Channel added in favourites:"));
    ui.label_stats_lexists_days->setText(tr("Channel exists days:"));

    // bold
    ui.pushButton_bold->setCheckable(true);

    // italic
    ui.pushButton_italic->setCheckable(true);
}

void DlgChannelSettings::set_default_values()
{
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
}

void DlgChannelSettings::create_signals()
{
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

    QObject::connect(ui.pushButton_permission_add, SIGNAL(clicked()), this, SLOT(button_permission_add()));
    QObject::connect(ui.pushButton_permission_remove, SIGNAL(clicked()), this, SLOT(button_permission_remove()));
    QObject::connect(ui.listWidget_permissions, SIGNAL(clicked(QModelIndex)), this, SLOT(change_permission_list(QModelIndex)));

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelSettings::set_channel(QString param1)
{
    strChannel = param1;
}

void DlgChannelSettings::set_data(QMap<QString, QString> mData)
{
    QMapIterator<QString, QString> i(mData);
    while (i.hasNext())
    {
        i.next();

        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "auditorium")
        {
            if (strValue.toInt() == 0)
                ui.radioButton_auditorium_off->setChecked(true);
            else if (strValue.toInt() == 1)
                ui.radioButton_auditorium_on->setChecked(true);
        }
        if (strKey == "catMajor")
        {
            if (strValue.toInt() == 1) // teen
            {
                ui.label_summary_category->setText(tr("Teen"));
                ui.comboBox_category->setCurrentIndex(0);
            }
            else if (strValue.toInt() == 2) // towarzyskie
            {
                ui.label_summary_category->setText(tr("Common"));
                ui.comboBox_category->setCurrentIndex(1);
            }
            else if (strValue.toInt() == 3) // erotyczne
            {
                ui.label_summary_category->setText(tr("Erotic"));
                ui.comboBox_category->setCurrentIndex(2);
            }
            else if (strValue.toInt() == 4) // tematyczne
            {
                ui.label_summary_category->setText(tr("Thematic"));
                ui.comboBox_category->setCurrentIndex(3);
            }
            else if (strValue.toInt() == 5) // regionalne
            {
                ui.label_summary_category->setText(tr("Regional"));
                ui.comboBox_category->setCurrentIndex(4);
            }
            else
                ui.comboBox_category->setCurrentIndex(-1);
        }
        else if (strKey == "createdDate")
        {
            // label created date/time
            QDateTime dt = QDateTime::fromTime_t(strValue.toInt());
            QString strDT = dt.toString("dd/MM/yyyy hh:mm:ss");
            ui.label_summary_datetime->setText(strDT);

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
        else if (strKey == "email")
        {
            ui.lineEdit_email->setText(strValue);
        }
        else if (strKey == "guardian")
        {
            if (strValue.toInt() == 0)
            {
                ui.label_summary_guardian->setText(tr("Inactive"));
                ui.radioButton_guardian_off->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(-1);
            }
            else if (strValue.toInt() == 1)
            {
                ui.label_summary_guardian->setText(QString("%1, %2").arg(tr("Active")).arg(tr("Level 1")));
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(0);
            }
            else if (strValue.toInt() == 2)
            {
                ui.label_summary_guardian->setText(QString("%1, %2").arg(tr("Active")).arg(tr("Level 2")));
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(1);
            }
            else if (strValue.toInt() == 3)
            {
                ui.label_summary_guardian->setText(QString("%1, %2").arg(tr("Active")).arg(tr("Level 3")));
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(2);
            }
        }
        else if (strKey == "limit")
        {
            ui.spinBox_limit->setValue(strValue.toInt());
        }
        else if (strKey == "moderated")
        {
            if (strValue.toInt() == 0)
            {
                ui.label_summary_moderation->setText(tr("Off"));
                ui.radioButton_moderation_off->setChecked(true);
            }
            else if (strValue.toInt() == 1)
            {
                ui.label_summary_moderation->setText(tr("On"));
                ui.radioButton_moderation_on->setChecked(true);
            }
        }
        else if (strKey == "password")
        {
            ui.lineEdit_password->setText(strValue);
        }
        else if (strKey == "private")
        {
            if (strValue.toInt() == 1)
            {
                ui.label_summary_status->setText(tr("Private"));
                ui.radioButton_status_priv->setChecked(true);
            }
            else if (strValue.toInt() == 0)
            {
                ui.label_summary_status->setText(tr("Public"));
                ui.radioButton_status_pub->setChecked(true);
            }
        }
        else if (strKey == "type")
        {
            if (strValue.toInt() == 0)
                ui.label_summary_type->setText(tr("Wild"));
            else if (strValue.toInt() == 1)
                ui.label_summary_type->setText(tr("Tame"));
            else if (strValue.toInt() == 2)
                ui.label_summary_type->setText(tr("With class"));
            else if (strValue.toInt() == 3)
                ui.label_summary_type->setText(tr("Cult"));
        }
        else if (strKey == "www")
        {
            if (strValue.isEmpty() == false)
                ui.label_summary_website->setText(QString("<a href=\"%1\">%1</a>").arg(strValue));
            ui.lineEdit_website->setText(strValue);
        }
    }
}

void DlgChannelSettings::set_stats_data(QMap<QString, QString> mData)
{
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
            for (int i = 0; i < strlWords.size(); i++)
            {
                bool ok;
                int iWord = strlWords.at(i).toInt(&ok, 16);

                // add to list
                lWords.append(iWord);

                // add to average words
                iWords += iWord;
            }

            // average words
            iWords /= strlWords.size();
            ui.label_stats_words->setText(QString::number(iWords));

            // simple stats widget
            pSimpleStatsWidget->set_stats(lWords);
        }
        else if (strKey == "relationsFavourite")
        {
            ui.label_stats_favourites->setText(strValue);
        }
    }
}

void DlgChannelSettings::enable_tabs()
{
    ui.tabWidget->setTabEnabled(1, true);
    ui.tabWidget->setTabEnabled(2, true);
    ui.tabWidget->setTabEnabled(3, true);
}

void DlgChannelSettings::set_topic(QString strTopic)
{
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
                    if (rx.exactMatch(strFont))
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

        if (strTopic.contains(strFontColor))
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

    // set summary topic
    if (strTopic.isEmpty() == false)
        ui.plainTextEdit_summary_topic->setPlainText(strTopic);

    // set topic
    ui.plainTextEdit_topic->setPlainText(strTopic);
}

void DlgChannelSettings::set_owner(QString strNick)
{
    ui.label_owner->show();
    ui.label_owner_nick->show();

    ui.label_owner_nick->setText(strNick);

    // enable tabs
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    if (strNick == strMe)
        enable_tabs();
}

void DlgChannelSettings::set_description(QString strDescription)
{
    // set summary description
    if (strDescription.isEmpty() == false)
    {
        QString strDescriptionText = strDescription;
        strDescriptionText.replace(QRegExp("%C([a-zA-Z0-9]+)%"),"");
        strDescriptionText.replace(QRegExp("%F([a-zA-Z0-9:]+)%"),"");
        strDescriptionText.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"//\\1");

        ui.plainTextEdit_summary_desc->setPlainText(strDescriptionText);
    }

    // set description
    ui.plainTextEdit_desc->setPlainText(strDescription);
}

void DlgChannelSettings::add_op(QString strNick)
{
    if (exist_item(strNick, ui.listWidget_op) == false)
    {
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setText(strNick);
        ui.listWidget_op->insertItem(ui.listWidget_op->count(), item);
    }

    // enable tabs
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    if (strNick == strMe)
        enable_tabs();
}

void DlgChannelSettings::add_halfop(QString strNick)
{
    if (exist_item(strNick, ui.listWidget_halfop) == false)
    {
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setText(strNick);
        ui.listWidget_halfop->insertItem(ui.listWidget_halfop->count(), item);
    }

    // enable tabs
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    if (strNick == strMe)
        enable_tabs();
}

void DlgChannelSettings::add_ban(QString strNick, QString strWho, QString strDT, QString strIPNick)
{
    if (exist_item(strNick, ui.listWidget_ban) == false)
    {
        SortedListWidgetItem *item = new SortedListWidgetItem();
        if (strIPNick.isEmpty())
        {
            item->setText(strNick);
            item->setToolTip(QString("%1: %2 (%4)").arg(tr("Created by")).arg(strWho).arg(strDT));
        }
        else
        {
            item->setText(strIPNick);
            item->setTextColor(QColor("#ff0000")); // set color
            item->setData(Qt::UserRole, strNick); // set original ban mask
            item->setToolTip(QString("%1: %2 (%3) [%4]").arg(tr("Created by")).arg(strWho).arg(strDT).arg(tr("IP Mask: %1")).arg(strNick.remove("*!*@")));
        }
        ui.listWidget_ban->insertItem(ui.listWidget_ban->count(), item);
    }
}

void DlgChannelSettings::add_invite(QString strNick, QString strWho, QString strDT)
{
    if (exist_item(strNick, ui.listWidget_invite) == false)
    {
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setText(strNick);
        item->setToolTip(QString("%1: %2 (%3)").arg(tr("Created by")).arg(strWho).arg(strDT));

        ui.listWidget_invite->insertItem(ui.listWidget_invite->count(), item);
    }
}

void DlgChannelSettings::owner_changed()
{
    QString strMsg = "<p style=\"font-weight:bold;\">"+tr("The owner of the channel can be only one!")+"</p><p>"+tr("Enter the nickname of the new owner:")+"</p>";
    bool ok;
    QString strNick = QInputDialog::getText(this, tr("Changing privileges"), strMsg, QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (strNick.isEmpty() == false))
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

    if ((ok) && (strText.isEmpty() == false))
        pNetwork->send(QString("CS DROP %1").arg(strText));

    strChannel.clear();
    clear();
    this->hide();
}

void DlgChannelSettings::email_changed()
{
    if (ui.lineEdit_email->text().isEmpty() == false)
        DlgEmail(Core::instance()->sccWindow(), strChannel, ui.lineEdit_email->text()).exec();
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
    if (ui.pushButton_bold->isChecked()) bBold = true;

    // italic
    if (ui.pushButton_italic->isChecked()) bItalic = true;

    // font name
    strFontName = ui.comboBox_font->currentText().toLower();

    // font color
    QStringList strlFontColors;
    strlFontColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    if (ui.comboBox_color->currentIndex() != -1)
        strFontColor = strlFontColors.at(ui.comboBox_color->currentIndex());

    // set topic
    if (bBold) strFontWeight += "b";
    if (bItalic) strFontWeight += "i";

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
    if (ui.radioButton_guardian_on->isChecked())
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

void DlgChannelSettings::button_permission_add()
{
    int tab = ui.stackedWidget->currentIndex();
    QString strNick;

    QString strLabel;
    if (tab == 0)
        strLabel = tr("Add the super-operator:");
    else if (tab == 1)
        strLabel = tr("Add operator:");
    else if (tab == 2)
        strLabel = tr("Add ban:");
    else if (tab == 3)
        strLabel = tr("Add invitation:");

    bool ok;
    QString strResult = QInputDialog::getText(this, tr("Changing privileges"), strLabel, QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (strResult.isEmpty() == false))
    {
        if ((tab == 2) && (strResult.contains("*")))
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(tr("This ban will ban large numbers of people. Are you sure you want to use this ban?"));
            int iResult = msgBox.exec();

            if (iResult == QMessageBox::Ok)
                strNick = strResult;
        }
        else
            strNick = strResult;
    }

    // if empty
    if (strNick.isEmpty())
        return;

    // add permission
    if (tab == 0)
        pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
    else if (tab == 1)
        pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
    else if (tab == 2)
        pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
    else if (tab == 3)
        pNetwork->send(QString("CS INVITE %1 ADD %2").arg(strChannel).arg(strNick));

    // clear
    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_permission_remove()
{
    int tab = ui.stackedWidget->currentIndex();
    QList<QListWidgetItem*> lRemoveNicks;

    if (tab == 0)
    {
        if (ui.listWidget_op->selectedItems().isEmpty() == false)
            lRemoveNicks = ui.listWidget_op->selectedItems();
    }
    else if (tab == 1)
    {
        if (ui.listWidget_halfop->selectedItems().isEmpty() == false)
            lRemoveNicks = ui.listWidget_halfop->selectedItems();
    }
    else if (tab == 2)
    {
        if (ui.listWidget_ban->selectedItems().isEmpty() == false)
            lRemoveNicks = ui.listWidget_ban->selectedItems();
    }
    else if (tab == 3)
    {
        if (ui.listWidget_invite->selectedItems().isEmpty() == false)
            lRemoveNicks = ui.listWidget_invite->selectedItems();
    }

    // get nick if empty
    if (lRemoveNicks.size() == 0)
    {
        QString strLabel;
        if (tab == 0)
            strLabel = tr("Remove the super-operator:");
        else if (tab == 1)
            strLabel = tr("Remove operator:");
        else if (tab == 2)
            strLabel = tr("Remove ban:");
        else if (tab == 3)
            strLabel = tr("Delete invitation:");

        bool ok;
        QString strNick = QInputDialog::getText(this, tr("Changing privileges"), strLabel, QLineEdit::Normal, QString::null, &ok);

        if ((ok) && (strNick.isEmpty() == false))
            lRemoveNicks.append(new QListWidgetItem(strNick));
    }

    // if empty
    if (lRemoveNicks.size() == 0)
        return;

    // remove permission
    if (tab == 0)
    {
        for (int i = 0; i < lRemoveNicks.size(); i++)
            pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(lRemoveNicks.at(i)->text()));
    }
    else if (tab == 1)
    {
        for (int i = 0; i < lRemoveNicks.size(); i++)
            pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(lRemoveNicks.at(i)->text()));
    }
    else if (tab == 2)
    {
        for (int i = 0; i < lRemoveNicks.size(); i++)
        {
            if (lRemoveNicks.at(i)->data(Qt::UserRole).isNull() == false)
                pNetwork->send(QString("CS BANIP %1 DEL %2").arg(strChannel).arg(lRemoveNicks.at(i)->text()));
            else
                pNetwork->send(QString("CS BAN %1 DEL %2").arg(strChannel).arg(lRemoveNicks.at(i)->text()));
        }
    }
    else if (tab == 3)
    {
        for (int i = 0; i < lRemoveNicks.size(); i++)
            pNetwork->send(QString("CS INVITE %1 DEL %2").arg(strChannel).arg(lRemoveNicks.at(i)->text()));
    }

    // clear
    clear();
    ui.label_channel_name->setText(strChannel);
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::change_permission_list(QModelIndex index)
{
    int row = index.row();
    ui.stackedWidget->setCurrentIndex(row);
}

void DlgChannelSettings::button_ok()
{
    this->hide();
}

void DlgChannelSettings::button_cancel()
{
    this->hide();
}

bool DlgChannelSettings::exist_item(QString find, QListWidget *list)
{
    QList<QListWidgetItem *> items = list->findItems(find, Qt::MatchExactly);
    if (items.size() != 0)
        return true;
    else
        return false;
}

void DlgChannelSettings::clear()
{
    ui.label_channel_name->clear();

    // summary
    ui.label_summary_datetime->setText("-");
    ui.label_summary_website->setText("-");
    ui.plainTextEdit_summary_topic->setPlainText("-");
    ui.plainTextEdit_summary_desc->setPlainText("-");
    ui.label_summary_status->setText("-");
    ui.label_summary_category->setText("-");
    ui.label_summary_type->setText("-");
    ui.label_summary_guardian->setText("-");
    ui.label_summary_moderation->setText("-");

    // general
    ui.label_owner_nick->clear();
    ui.lineEdit_website->clear();
    ui.plainTextEdit_topic->clear();
    ui.pushButton_bold->setChecked(false);
    ui.pushButton_italic->setChecked(false);
    ui.comboBox_font->setCurrentIndex(-1);
    ui.comboBox_color->setCurrentIndex(-1);
    ui.plainTextEdit_desc->clear();
    ui.radioButton_status_pub->setChecked(false);
    ui.radioButton_status_priv->setChecked(false);
    ui.comboBox_category->setCurrentIndex(-1);
    ui.comboBox_guardian_level->setCurrentIndex(-1);
    ui.radioButton_guardian_off->setChecked(false);
    ui.radioButton_guardian_on->setChecked(false);
    ui.radioButton_moderation_off->setChecked(false);
    ui.radioButton_moderation_on->setChecked(false);

    // advanced
    ui.lineEdit_email->clear();
    ui.lineEdit_password->clear();
    ui.spinBox_limit->setValue(0);
    ui.radioButton_auditorium_off->setChecked(false);
    ui.radioButton_auditorium_on->setChecked(false);

    // permissions
    ui.listWidget_permissions->setCurrentRow(0);
    ui.listWidget_op->clear();
    ui.listWidget_halfop->clear();
    ui.listWidget_ban->clear();
    ui.listWidget_invite->clear();

    // statistics
    ui.label_stats_favourites->setText("-");
    ui.label_stats_words->setText("-");
    ui.label_stats_exists_days->setText("-");
}

void DlgChannelSettings::showEvent(QShowEvent *event)
{
    event->accept();
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    ui.label_channel_name->setText(strChannel);

    // disabled
    ui.tabWidget->setTabEnabled(1, false);
    ui.tabWidget->setTabEnabled(2, false);
    ui.tabWidget->setTabEnabled(3, false);

    // get data
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
    pNetwork->send(QString("RS INFO %1").arg(strChannel));
}

void DlgChannelSettings::hideEvent(QHideEvent *event)
{
    event->accept();

    // clear
    strChannel.clear();
    clear();
    pSimpleStatsWidget->clear_stats();
    ui.label_owner->hide();
    ui.label_owner_nick->hide();

    // switch tab
    ui.tabWidget->setCurrentIndex(0);
    ui.stackedWidget->setCurrentIndex(0);
}

void DlgChannelSettings::closeEvent(QCloseEvent *event)
{
    event->ignore();

    this->hide();
}
