/*

  This file is part of Simple Chat Client

  Copyright (c) 2010-2011 by Frank Schaefer <fschaefer.oss@googlemail.com>
  Copyright (c) 2005-2006 by Cláudio da Silveira Pinheiro <taupter@gmail.com>
  Kopete (c) 2002-2003 by the Kopete developers <kopete-devel@kde.org>

*************************************************************************
*                                                                       *
* This library is free software; you can redistribute it and/or         *
* modify it under the terms of the GNU Lesser General Public            *
* License as published by the Free Software Foundation; either          *
* version 2 of the License, or (at your option) any later version.      *
*                                                                       *
*************************************************************************
*/

#ifndef VIDEO_H
#define VIDEO_H

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <QImage>
#include <QObject>

typedef enum
{
    // Packed RGB formats
    PIXELFORMAT_NONE = 0,
    PIXELFORMAT_GREY = (1 << 0),
    PIXELFORMAT_RGB332 = (1 << 1),
    PIXELFORMAT_RGB444 = (1 << 2),
    PIXELFORMAT_RGB555 = (1 << 3),
    PIXELFORMAT_RGB565 = (1 << 4),
    PIXELFORMAT_RGB555X = (1 << 5),
    PIXELFORMAT_RGB565X = (1 << 6),
    PIXELFORMAT_BGR24 = (1 << 7),
    PIXELFORMAT_RGB24 = (1 << 8),
    PIXELFORMAT_BGR32 = (1 << 9),
    PIXELFORMAT_RGB32 = (1 << 10),

    // Bayer RGB format
    PIXELFORMAT_SBGGR8 = (1 << 11),

    // YUV formats
    PIXELFORMAT_YUYV = (1 << 12),
    PIXELFORMAT_UYVY = (1 << 13),
    PIXELFORMAT_YUV420P = (1 << 14),
    PIXELFORMAT_YUV422P = (1 << 15),

    // Compressed formats
    PIXELFORMAT_JPEG = (1 << 16),
    PIXELFORMAT_MPEG = (1 << 17),

    // Reserved formats
    PIXELFORMAT_DV = (1 << 18),
    PIXELFORMAT_ET61X251 = (1 << 19),
    PIXELFORMAT_HI240 = (1 << 20),
    PIXELFORMAT_HM12 = (1 << 21),
    PIXELFORMAT_MJPEG = (1 << 22),
    PIXELFORMAT_PWC1 = (1 << 23),
    PIXELFORMAT_PWC2 = (1 << 24),
    PIXELFORMAT_SN9C10X = (1 << 25),
    PIXELFORMAT_WNVA = (1 << 26),
    PIXELFORMAT_YYUV = (1 << 27)

    // PIXELFORMAT_ALL = 0x00003FFF
} pixel_format;

struct imagebuffer
{
    int height;
    int width;
    pixel_format pixelformat;
    QVector <uchar> data;
};

struct rawbuffer // raw buffer
{
    uchar * start;
    size_t length;
};


class Video : public QObject
{
    Q_OBJECT
public:
    Video();
    ~Video();
    inline bool is_created() { return bCreatedCapture; }

    void create();
    void destroy();
    void get_image(QImage *);

    bool exist_video_device();
    QList<QString> get_video_devices();

private:
    bool bCreatedCapture;
    char* full_filename;
    pixel_format m_pixelformat;
    imagebuffer m_currentbuffer;
    QVector<rawbuffer> m_rawbuffers;
    unsigned int m_streambuffers;
    int m_buffer_size;
    int descriptor;
    struct v4l2_capability V4L2_capabilities;
    struct v4l2_format fmt;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    int width;
    int height;

    int errnoReturn(const char* s);
    int xioctl(int request, void *arg);

    void open_device();
    void init_device();
    void init_mmap();
    void start_capturing();
    void get_frame();
    void stop_capturing();
    void uninit_device();
    void close_device();
};

#endif // VIDEO_H
