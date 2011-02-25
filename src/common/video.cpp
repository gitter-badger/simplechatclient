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

#include <QDebug>
#include <QFile>
#include <QImage>
#include "video.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

Video::Video()
{
    bCreatedCapture = false;
    full_filename = "/dev/video0";
    descriptor = -1;
    m_streambuffers = 0;
    width = 320;
    height = 240;
}

Video::~Video()
{
    destroy();
}

int Video::errnoReturn(const char* s)
{
    fprintf (stderr, "%s error %d, %s\n",s, errno, strerror (errno));
    return EXIT_FAILURE;
}

int Video::xioctl(int request, void *arg)
{
    int r;

    do r = ioctl (descriptor, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

void Video::create()
{
    if (bCreatedCapture == false)
    {
        open_device();
        init_device();
        init_mmap();
        start_capturing();

        bCreatedCapture = true;
    }
}

void Video::destroy()
{
    // destroy capture
    if (bCreatedCapture == true)
    {
        stop_capturing();
        close_device();

        bCreatedCapture = false;
    }
}

void Video::get_frame()
{
    struct v4l2_buffer v4l2buffer;

    CLEAR (v4l2buffer);
    v4l2buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2buffer.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl (VIDIOC_DQBUF, &v4l2buffer))
    {
        switch (errno)
        {
            case EAGAIN:
            {
                //qDebug() << "Video: No new frame available.";
                return;
            }
            case EIO: /* Could ignore EIO, see spec. fall through */
            default:
                errnoReturn ("VIDIOC_DQBUF");
        }
    }

    //if (v4l2buffer.index < m_streambuffers)
    //return EXIT_FAILURE; //it was an assert()

    memcpy(&m_currentbuffer.data[0], m_rawbuffers[v4l2buffer.index].start, m_currentbuffer.data.size());
    if (-1 == xioctl (VIDIOC_QBUF, &v4l2buffer))
        errnoReturn ("VIDIOC_QBUF");
}

void Video::get_image(QImage *qimage)
{
    get_frame();

// do NOT delete qimage here, as it is received as a parameter
    if (qimage->width() != width || qimage->height() != height)
        *qimage = QImage(width, height, QImage::Format_RGB32);

    uchar *bits=qimage->bits();

    uchar *yptr, *cbptr, *crptr;
    bool halfheight=false;
    bool packed=false;
    // Adjust algorythm to specific YUV data arrangements.
    yptr = &m_currentbuffer.data[0];
    cbptr = yptr + 1;
    crptr = yptr + 3;
    packed=true;

    for (int y=0; y<height; y++)
    {
        // Decode scanline
        for (int x=0; x<width; x++)
        {
            int c,d,e;

            if (packed)
            {
                c = (yptr[x<<1])-16;
                d = (cbptr[x>>1<<2])-128;
                e = (crptr[x>>1<<2])-128;
            }
            else
            {
                c = (yptr[x])-16;
                d = (cbptr[x>>1])-128;
                e = (crptr[x>>1])-128;
            }

            int r = (298 * c + 409 * e + 128)>>8;
            int g = (298 * c - 100 * d - 208 * e + 128)>>8;
            int b = (298 * c + 516 * d + 128)>>8;

            if (r<0) r=0; if (r>255) r=255;
            if (g<0) g=0; if (g>255) g=255;
            if (b<0) b=0; if (b>255) b=255;

            uint *p = (uint*)qimage->scanLine(y)+x;
            *p = qRgba(r,g,b,255);

        }
        // Jump to next line
        if (packed)
        {
            yptr+=width*2;
            cbptr+=width*2;
            crptr+=width*2;
        }
        else
        {
            yptr+=width;
            if (!halfheight || y&1)
            {
                cbptr+=width/2;
                crptr+=width/2;
            }
        }
    }

    // Proccesses image for automatic Brightness/Contrast/Color correction
    //if (getAutoBrightnessContrast()||getAutoColorCorrection())
    //{
    unsigned long long R=0, G=0, B=0, global=0;
    int Rmax=0, Gmax=0, Bmax=0, globalmax=0;
    int Rmin=255, Gmin=255, Bmin=255, globalmin=255;
    int Rrange=255, Grange=255, Brange=255;

    // Finds minimum and maximum intensity for each color component
    for(int loop=0;loop < qimage->numBytes();loop+=4)
    {
        R+=bits[loop];
        G+=bits[loop+1];
        B+=bits[loop+2];
        // A+=bits[loop+3];
        if (bits[loop] < Rmin) Rmin = bits[loop];
        if (bits[loop+1] < Gmin) Gmin = bits[loop+1];
        if (bits[loop+2] < Bmin) Bmin = bits[loop+2];
        // if (bits[loop+3] < Amin) Amin = bits[loop+3];
        if (bits[loop] > Rmax) Rmax = bits[loop];
        if (bits[loop+1] > Gmax) Gmax = bits[loop+1];
        if (bits[loop+2] > Bmax) Bmax = bits[loop+2];
        // if (bits[loop+3] > Amax) Amax = bits[loop+3];
    }
    global = R + G + B;
    // Finds overall minimum and maximum intensity
    if (Rmin > Gmin) globalmin = Gmin; else globalmin = Rmin; if (Bmin < globalmin) globalmin = Bmin;
    if (Rmax > Gmax) globalmax = Rmax; else globalmax = Gmax; if (Bmax > globalmax) globalmax = Bmax;
    // If no color correction should be performed, simply level all the intensities so they're just the same.
    // In fact color correction should use the R, G and B variables to detect color deviation and "bump up" the saturation,
    // but it's computationally more expensive and the current way returns better results to the user.

    //if(!getAutoColorCorrection())
    //{
    Rmin = globalmin ; Rmax = globalmax;
    Gmin = globalmin ; Gmax = globalmax;
    Bmin = globalmin ; Bmax = globalmax;
    // Amin = globalmin ; Amax = globalmax;
    //}
    // Calculates ranges and prevent a division by zero later on.
    Rrange = Rmax - Rmin; if (Rrange == 0) Rrange = 255;
    Grange = Gmax - Gmin; if (Grange == 0) Grange = 255;
    Brange = Bmax - Bmin; if (Brange == 0) Brange = 255;
    // Arange = Amax - Amin; if (Arange == 0) Arange = 255;

    for(int loop=0;loop < qimage->numBytes();loop+=4)
    {
        bits[loop] = (bits[loop] - Rmin) * 255 / (Rrange);
        bits[loop+1] = (bits[loop+1] - Gmin) * 255 / (Grange);
        bits[loop+2] = (bits[loop+2] - Bmin) * 255 / (Brange);
        // bits[loop+3] = (bits[loop+3] - Amin) * 255 / (Arange);
    }
    //}

}

void Video::open_device()
{
    if(-1 != descriptor)
    {
        qDebug() << "Video Error: Device is already open";
        return;
    }
    descriptor = ::open (QFile::encodeName(full_filename), O_RDWR | O_NONBLOCK, 0);

    if (-1 == descriptor)
        qDebug() << "Video Error: Cannot open file " << full_filename;
}

void Video::init_device()
{
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (VIDIOC_CROPCAP, &cropcap))
    {
        // Errors ignored.
    }
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; // reset to default
    if (-1 == xioctl (VIDIOC_S_CROP, &crop))
    {
        switch (errno)
        {
            case EINVAL: break; // Cropping not supported.
            default: break; // Errors ignored.
        }
    }

    //default V4L2_PIX_FMT_YUYV
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    m_pixelformat = PIXELFORMAT_YUYV;

    // CLEAR (fmt);
    if (-1 == xioctl (VIDIOC_G_FMT, &fmt))
    {
        // ignore
        //qDebug() << "Video: VIDIOC_G_FMT failed (" << errno << ").Returned width: " << fmt.fmt.pix.width << "x" << fmt.fmt.pix.height;
    }
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;

    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;//V4L2_FIELD_ANY;
    if (-1 == xioctl (VIDIOC_S_FMT, &fmt))
    {
        // ignore
        //qDebug() << "Video Error: VIDIOC_S_FMT failed (" << errno << ").Returned width: " << fmt.fmt.pix.width << "x" << fmt.fmt.pix.height;
    }
    else
    {
        // Buggy driver paranoia.
        //qDebug() << "Video Error: VIDIOC_S_FMT worked (" << errno << ").Returned width: " << fmt.fmt.pix.width << "x" << fmt.fmt.pix.height;
        unsigned int min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
            fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
            fmt.fmt.pix.sizeimage = min;
        m_buffer_size=fmt.fmt.pix.sizeimage ;
    }

    m_buffer_size = width * height * 16 / 8;

    m_currentbuffer.pixelformat=m_pixelformat;
    m_currentbuffer.data.resize(m_buffer_size);
}

void Video::init_mmap()
{
    struct v4l2_requestbuffers req;

    #define BUFFERS 2
    CLEAR (req);

    req.count = BUFFERS;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (VIDIOC_REQBUFS, &req))
    {
        if (EINVAL == errno)
        {
            qDebug() << "Video Error: " << full_filename << " does not support memory mapping";
            return;
        }
        else
        {
            errnoReturn ("VIDIOC_REQBUFS");
        }
    }

    if (req.count < BUFFERS)
    {
        qDebug() << "Video Error: Insufficient buffer memory on " << full_filename;
        return;
    }

    m_rawbuffers.resize(req.count);

    if (m_rawbuffers.size()==0)
    {
        qDebug() << "Video Error: Out of memory";
        return;
    }

    for (m_streambuffers = 0; m_streambuffers < req.count; ++m_streambuffers)
    {
        struct v4l2_buffer v4l2buffer;

        CLEAR (v4l2buffer);

        v4l2buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2buffer.memory = V4L2_MEMORY_MMAP;
        v4l2buffer.index = m_streambuffers;

        if (-1 == xioctl (VIDIOC_QUERYBUF, &v4l2buffer))
            errnoReturn ("VIDIOC_QUERYBUF");

        m_rawbuffers[m_streambuffers].length = v4l2buffer.length;
        m_rawbuffers[m_streambuffers].start = (uchar *) mmap (NULL /* start anywhere */, v4l2buffer.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, descriptor, v4l2buffer.m.offset);

        if (MAP_FAILED == m_rawbuffers[m_streambuffers].start)
            errnoReturn ("mmap");
    }

    m_currentbuffer.data.resize(m_rawbuffers[0].length); // Makes the imagesize.data buffer size equal to the rawbuffer size
}

void Video::start_capturing()
{
    unsigned int loop;
    for (loop = 0; loop < m_streambuffers; ++loop)
    {
        struct v4l2_buffer buf;
        CLEAR (buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = loop;
        if (-1 == xioctl (VIDIOC_QBUF, &buf))
            errnoReturn ("VIDIOC_QBUF");
    }
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (VIDIOC_STREAMON, &type))
        errnoReturn ("VIDIOC_STREAMON");
}

void Video::stop_capturing()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (VIDIOC_STREAMOFF, &type))
        errnoReturn ("VIDIOC_STREAMOFF");

    unsigned int loop;
    for (loop = 0; loop < m_streambuffers; ++loop)
    {
        if (munmap(m_rawbuffers[loop].start,m_rawbuffers[loop].length) != 0)
        {
            qDebug() << "Video Error: unable to munmap.";
        }
    }
}

void Video::close_device()
{
    stop_capturing();
    descriptor = -1;
}

bool Video::exist_video_device()
{
    // search video device
    for (int i = 0; i < 10; ++i)
    {
        if (QFile::exists("/dev/video"+QString::number(i)) == true)
            return true;
    }
    return false;
}

QList<QString> Video::get_video_devices()
{
    QList<QString> lVideoDevices;

    struct v4l2_capability cap;

    // get video devices
    for (int i = 0; i < 10; ++i)
    {
        QString strDevice;
        strDevice.sprintf("/dev/video%d",i);
        int iFileDescriptor = open(strDevice.toAscii(), O_RDWR | O_NONBLOCK, 0);

        if (iFileDescriptor > -1)
        {
            if (ioctl(iFileDescriptor, VIDIOC_QUERYCAP, &cap) != -1)
            {
                QString strName;
                strName.sprintf("%s", cap.card);
                close(iFileDescriptor);

                // add name
                lVideoDevices.append(strName);
            }
        }
    }
    return lVideoDevices;
}
