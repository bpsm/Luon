#ifndef STDISPLAY_H
#define STDISPLAY_H

/*
* Copyright 2020 Rochus Keller <mailto:me@rochus-keller.ch>
*
* This file is part of the Smalltalk parser/compiler library.
*
* The following is the license that applies to this copy of the
* library. For a license to use the library under conditions
* other than those described here, please email to me@rochus-keller.ch.
*
* GNU General Public License Usage
* This file may be used under the terms of the GNU General Public
* License (GPL) versions 2.0 or 3.0 as published by the Free Software
* Foundation and appearing in the file LICENSE.GPL included in
* the packaging of this file. Please review the following information
* to ensure GNU General Public Licensing requirements will be met:
* http://www.fsf.org/licensing/licenses/info/GPLv2.html and
* http://www.gnu.org/copyleft/gpl.html.
*/

// Adopted from StDisplay.cpp/h from the Smalltalk repository

#include <QElapsedTimer>
#include <QFile>
#include <QQueue>
#include <QWidget>


namespace St
{
    class Bitmap
    {
    public:
        enum { PixPerByte = 8, PixPerWord = PixPerByte * 2 };
        Bitmap():d_buf(0),d_wordLen(0),d_pixWidth(0),d_pixHeight(0),d_pixLineWidth(0) {}
        Bitmap( quint8* buf, quint16 wordLen, quint16 pixWidth, quint16 pixHeight );
        inline const quint8* scanLine(int y) const
        {
            return d_buf + ( y * d_pixLineWidth / PixPerByte );
        }
        quint16 lineWidth() const { return d_pixLineWidth; }
        quint16 width() const { return d_pixWidth; }
        quint16 height() const { return d_pixHeight; }
        quint16 wordLen() const { return d_wordLen; }
        inline quint16 wordAt(quint16 i) const
        {
            i--; // Smalltalk array indexes start with 1
            Q_ASSERT( i < d_wordLen );
            return readU16( d_buf, i * 2 );
        }
        void wordAtPut( quint16 i, quint16 v );
        bool isNull() const { return d_buf == 0; }
        bool isSameBuffer( const Bitmap& rhs ) const { return rhs.d_buf == d_buf; }
        void toImage(QImage&, QRect = QRect()) const;
    protected:
        static inline quint16 readU16( const quint8* data, int off )
        {
            return ( quint8(data[off]) << 8 ) + quint8(data[off+1] );
        }

    private:
        quint16 d_pixWidth, d_pixHeight, d_pixLineWidth, d_wordLen;
        quint8* d_buf;
    };

    class Display : public QWidget
    {
        Q_OBJECT
    public:
        typedef void (*EventCallback)();
        enum EventType {
            DeltaTime = 0,
            XLocation = 1,
            YLocation = 2,
            BiStateOn = 3,
            BiStateOff = 4,
            AbsoluteTime = 5, // followed by 2 words
        };
        enum { MaxPos = 0xfff }; // 12 bits

        explicit Display(QWidget *parent = 0);
        ~Display();
        static Display* inst();
        static void forceClose();
        static bool s_run;
        static bool s_break;
        static bool s_copy;
        static QList<QFile*> s_files;

        void setBitmap( const Bitmap& );
        const Bitmap& getBitmap() const { return d_bitmap; }
        void setCursorBitmap( const Bitmap& );
        void setCursorPos( qint16 x, qint16 y );
        const QPoint& getMousePos() const { return d_mousePos; }
        quint16 nextEvent() { return d_events.dequeue(); }
        void clearEvents() { d_events.clear(); }
        int eventsPending() const { return d_events.size(); }
        void drawRecord( int x, int y, int w, int h );
        bool isRecOn() const { return d_recOn; }
        void updateArea(const QRect& r);
        void setLog(bool on);
        const QImage& getScreen() const { return d_screen; }
        static void processEvents();
        static void copyToClipboard( const QByteArray& );
    signals:
        void sigEventQueue();

    protected slots:
        void onRecord();
        void onExit();
        void onLog();
        void onBreak();
        void onPaste();
        void onCopy();
        void onPasteBenchmark();

    protected:
        void paintEvent(QPaintEvent *);
        void timerEvent(QTimerEvent *);
        void closeEvent(QCloseEvent * event);
        void mouseMoveEvent(QMouseEvent * event);
        void mousePressEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mousePressReleaseImp(bool press, int button );
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);
        void inputMethodEvent(QInputMethodEvent *);
        QString renderTitle() const;
        bool postEvent(EventType, quint16 param = 0 , bool withTime = true);
        bool keyEvent( int keyCode, char ch, bool down );
        void simulateKeyEvent( char ch );
        void sendShift(bool keyPress, bool shiftRequired);
        void notify();
        void updateImage();
    private:
        Bitmap d_bitmap;
        QImage d_screen;
        QImage d_cursor;
        qint16 d_curX, d_curY;
        QPoint d_mousePos;
        QQueue<quint16> d_events;
        quint32 d_lastEvent; // number of milliseconds since last event was posted to queue
        QImage d_record;
        QRect d_updateArea;
        bool d_shiftDown, d_capsLockDown, d_recOn, d_forceClose;
    };
}

#endif // STDISPLAY_H
