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
            // set current emoticon
            strCurrentEmoticon = strEmoticon;

            // stop movie
            if (movie->state() == QMovie::Running)
                movie->stop();

            // set width, height
            QImage im(strCurrentEmoticon);
            int w = im.width();
            int h = im.height();
            this->label->setMinimumSize(w,h);
            this->label->setMaximumSize(w,h);
            // set width, height
            this->adjustSize(); // fix bug

            // movie
            label->clear();
            movie->setFileName(strCurrentEmoticon);
            label->setMovie(movie);
            movie->start();

            // move widget
            int img_begin_x = get_correct_x(x, y);
            int img_begin_y = get_correct_y(x, y);

            int fix_x = (this->frameGeometry().width() - w)/2;
            int fix_y = (this->frameGeometry().height() - h)/2;

#ifdef Q_WS_WIN
            fix_y--;
#endif

            // move
            this->move(img_begin_x-w-fix_x, img_begin_y-h-fix_y);

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
