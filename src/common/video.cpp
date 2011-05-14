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

#ifdef HAVE_LIBV4L2
    do r = v4l2_ioctl (descriptor, request, arg);
#else
    do r = ioctl (descriptor, request, arg);
#endif
    while (-1 == r && EINTR == errno);

    return r;
}

void Video::create()
{
    if (!bCreatedCapture)
    {
        openDevice();
        initDevice();
        initMmap();
        startCapturing();

        bCreatedCapture = true;
    }
}

void Video::destroy()
{
    // destroy capture
    if (bCreatedCapture)
    {
        stopCapturing();
        closeDevice();

        bCreatedCapture = false;
    }
}

int Video::getFrame()
{
    struct v4l2_buffer v4l2buffer;

    CLEAR (v4l2buffer);
    v4l2buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2buffer.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl (VIDIOC_DQBUF, &v4l2buffer))
    {
        if (errno == EAGAIN)
        {
            //qDebug() << "Video: No new frame available.";
            return EXIT_FAILURE;
        }
        else
            return errnoReturn ("VIDIOC_DQBUF");
    }

    if (m_currentbuffer.data.isEmpty() ||
        // v4l2buffer.index < 0 || // is always false: v4l2buffer.index is unsigned
        (uint) m_rawbuffers.size() <= v4l2buffer.index)
        return EXIT_FAILURE;

    if (m_rawbuffers[v4l2buffer.index].length < (uint)m_currentbuffer.data.size())
    {
        qDebug() << "Buffer size mismatch: expecting raw buffer length to be" << m_currentbuffer.data.size() << "but it was" << m_rawbuffers[v4l2buffer.index].length;
        return EXIT_FAILURE;
    }

    memcpy(&m_currentbuffer.data[0], m_rawbuffers[v4l2buffer.index].start, m_currentbuffer.data.size());
    if (-1 == xioctl (VIDIOC_QBUF, &v4l2buffer))
        return errnoReturn ("VIDIOC_QBUF");

    return EXIT_SUCCESS;
}

void Video::getImage(QImage *qimage)
{
    int g = getFrame();
    if (g == EXIT_FAILURE) return; // no new image; prevent crash

// do NOT delete qimage here, as it is received as a parameter
    if (qimage->width() != width || qimage->height() != height)
        *qimage = QImage(width, height, QImage::Format_RGB32);

    if (!m_currentbuffer.data.size())
        return; // EXIT_FAILURE

    uchar *yptr, *cbptr, *crptr;
    bool halfheight=false;
    bool packed=false;
    // Adjust algorythm to specific YUV data arrangements.
    if (m_currentbuffer.pixelformat == PIXELFORMAT_YUV420P)
        halfheight=true;
    if (m_currentbuffer.pixelformat == PIXELFORMAT_YUYV)
    {
        yptr = &m_currentbuffer.data[0];
        cbptr = yptr + 1;
        crptr = yptr + 3;
        packed=true;
    }
    else if (m_currentbuffer.pixelformat == PIXELFORMAT_UYVY)
    {
        cbptr = &m_currentbuffer.data[0];
        yptr = cbptr + 1;
        crptr = cbptr + 2;
        packed=true;
    }
    else
    {
        yptr = &m_currentbuffer.data[0];
        cbptr = yptr + (width*height);
        crptr = cbptr + (width*height/(halfheight ? 4:2));
    }

    for(int y=0; y<height; y++)
    {
    // Decode scanline
        for(int x=0; x<width; x++)
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
}

int Video::openDevice()
{
    if(-1 != descriptor)
    {
        qDebug() << "Video Error: Device is already open";
        return EXIT_FAILURE;
    }

#ifdef HAVE_LIBV4L2
    descriptor = ::v4l2_open (QFile::encodeName(full_filename), O_RDWR | O_NONBLOCK, 0);
#else
    descriptor = ::open (QFile::encodeName(full_filename), O_RDWR | O_NONBLOCK, 0);
#endif

    if(isOpen())
    {
        qDebug() << "File " << full_filename << " was opened successfuly";
/*
        if(EXIT_FAILURE==checkDevice())
        {
            qDebug() << "File " << full_filename << " could not be opened";
            closeDevice();
            return EXIT_FAILURE;
        }
*/
    }
    else
    {
        qDebug() << "Unable to open file " << full_filename << "Err: "<< errno;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool Video::isOpen()
{
    if(-1 == descriptor)
    {
        // qDebug() << "VideoDevice::isOpen() File is not open";
        return false;
    }

    // qDebug() << "VideoDevice::isOpen() File is open";
    return true;
}

void Video::initDevice()
{
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    CLEAR (cropcap);
    CLEAR (crop);

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
            default:     break; // Errors ignored.
        }
    }

    // setSize()

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
    fmt.fmt.pix.field = V4L2_FIELD_ANY;
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

    m_currentbuffer.width=320;
    m_currentbuffer.height=240;
    m_currentbuffer.pixelformat=m_pixelformat;
    m_currentbuffer.data.resize(m_buffer_size);
}

int Video::initMmap()
{
    #define BUFFERS 2

    struct v4l2_requestbuffers req;
    CLEAR (req);

    req.count = BUFFERS;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (VIDIOC_REQBUFS, &req))
    {
        if (EINVAL == errno)
        {
            qDebug() << "Video Error: " << full_filename << " does not support memory mapping";
            return EXIT_FAILURE;
        }
        else
        {
            return errnoReturn ("VIDIOC_REQBUFS");
        }
    }

    if (req.count < BUFFERS)
    {
        qDebug() << "Video Error: Insufficient buffer memory on " << full_filename;
        return EXIT_FAILURE;
    }

    m_rawbuffers.resize(req.count);

    if (m_rawbuffers.size()==0)
    {
        qDebug() << "Video Error: Out of memory";
        return EXIT_FAILURE;
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
#ifdef HAVE_LIBV4L2
        m_rawbuffers[m_streambuffers].start = (uchar *) v4l2_mmap (NULL /* start anywhere */, v4l2buffer.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, descriptor, v4l2buffer.m.offset);
#else
        m_rawbuffers[m_streambuffers].start = (uchar *) mmap (NULL /* start anywhere */, v4l2buffer.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, descriptor, v4l2buffer.m.offset);
#endif

        if (MAP_FAILED == m_rawbuffers[m_streambuffers].start)
            return errnoReturn ("mmap");
    }

    m_currentbuffer.data.resize(m_rawbuffers[0].length); // Makes the imagesize.data buffer size equal to the rawbuffer size

    return EXIT_SUCCESS;
}

int Video::startCapturing()
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
            return errnoReturn ("VIDIOC_QBUF");
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (VIDIOC_STREAMON, &type))
        return errnoReturn ("VIDIOC_STREAMON");

    return EXIT_SUCCESS;
}

int Video::stopCapturing()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (VIDIOC_STREAMOFF, &type))
        return errnoReturn ("VIDIOC_STREAMOFF");

    unsigned int loop;
    for (loop = 0; loop < m_streambuffers; ++loop)
    {
#ifdef HAVE_LIBV4L2
        if (v4l2_munmap(m_rawbuffers[loop].start,m_rawbuffers[loop].length) != 0)
#else
        if (munmap(m_rawbuffers[loop].start,m_rawbuffers[loop].length) != 0)
#endif
        {
            qDebug() << "Video Error: unable to munmap.";
        }
    }

    return EXIT_SUCCESS;
}

void Video::closeDevice()
{
    stopCapturing();
    descriptor = -1;
}

bool Video::existVideoDevice()
{
    // search video device
    for (int i = 0; i < 10; ++i)
    {
        if (QFile::exists("/dev/video"+QString::number(i)))
            return true;
    }
    return false;
}

QList<QString> Video::getVideoDevices()
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
