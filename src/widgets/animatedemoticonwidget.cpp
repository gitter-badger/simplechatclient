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

#include <QSettings>
#include "animatedemoticonwidget.h"

AnimatedEmoticonWidget::AnimatedEmoticonWidget(QTextEdit *parent) : QWidget(parent)
{
    parentWidget = parent;

    QSettings settings;
    QString strBackgroundColor = settings.value("background_color").toString();
    this->setStyleSheet(QString("background-color:%1").arg(strBackgroundColor));

    label = new QLabel();
    movie = new QMovie();
    layout = new QVBoxLayout();
    layout->addWidget(label);
    this->setLayout(layout);
}

void AnimatedEmoticonWidget::start_emoticon(QString strEmoticon, int x, int y)
{
    if (strEmoticon != strCurrentEmoticon)
    {
        QMovie isAnimated(strEmoticon);
        if (isAnimated.frameCount() > 1)
        {
            strCurrentEmoticon = strEmoticon;
            if (movie->state() == QMovie::Running)
                movie->stop();
            label->clear();
            movie->setFileName(strCurrentEmoticon);
            label->setMovie(movie);
            movie->start();

            // move widget
            int new_x = get_correct_x(x, y);
            int new_y = get_correct_y(x, y);
            QImage im(strCurrentEmoticon);
            int w = im.width();
            int h = im.height();
            // move
            this->move(new_x-w, new_y-h);

            // fix size
            this->adjustSize();

            // show widget
            this->show();
        }
    }
}

void AnimatedEmoticonWidget::stop_emoticon()
{
    if (movie->state() == QMovie::Running)
        movie->stop();
    if (this->isHidden() == false)
        this->hide();
    if (strCurrentEmoticon.isEmpty() == false)
        strCurrentEmoticon.clear();
}

int AnimatedEmoticonWidget::get_correct_x(int x, int y)
{
    bool bIsXImage = true;
    while(bIsXImage == true)
    {
        x++;
        QTextCursor cursor = parentWidget->cursorForPosition(QPoint(x,y));
        QTextFormat format = cursor.charFormat();

        if (!format.isImageFormat())
        {
            bIsXImage = false;
            // fix x
            x--;
        }
    }
    return x;
}

int AnimatedEmoticonWidget::get_correct_y(int x, int y)
{
    bool bIsYImage = true;
    while(bIsYImage == true)
    {
        y++;
        QTextCursor cursor = parentWidget->cursorForPosition(QPoint(x,y));
        QTextFormat format = cursor.charFormat();

        if (!format.isImageFormat())
        {
            bIsYImage = false;
            // fix y
            y--;
        }
    }
    return y;
}
