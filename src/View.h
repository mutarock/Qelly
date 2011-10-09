/*****************************************************************************
 * View.h
 *
 * Created: 06/10 2011 by uranusjr
 *
 * Copyright 2011 uranusjr. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#ifndef VIEW_H
#define VIEW_H

#include "UJQxWidget.h"
#include <QPainter>
#include <QQueue>
#include <QVector>
#include "Globals.h"
class QTimer;

namespace UJ
{

namespace Connection
{
class Terminal;
}

namespace Qelly
{

class View : public Qx::Widget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    virtual ~View();
    bool needBlinking();
    void insertText(QString &string, uint delayMs = 0);

public slots:
    void updateScreen();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseTripleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void inputMethodEvent(QInputMethodEvent *e);
    void paintEvent(QPaintEvent *e);

signals:
    void hasBytesToSend(QByteArray bytes);

private slots:
    inline void displayCellAt(int row, int column)
    {
        update(column * _cellWidth, row * _cellHeight, _cellWidth, _cellHeight);
    }
    void updateBackImage();
    void updateBackground(int row, int startColumn, int endColumn);
    void updateText(int row);
    void updateText(int row, int x);
    void paintSpecialSymbol(ushort code, int row, int column,
                            BBS::CellAttribute left, BBS::CellAttribute right);
    void paintDoubleColor(ushort code, int row, int column,
                          BBS::CellAttribute left, BBS::CellAttribute right);
    void drawSelection(QRect &r);
    void drawBlink(QRect &r);
    void refreshHiddenRegion();
    void clearSelection();
    void extendBottom(int start, int end);
    void extendTop(int start, int end);
    void popInsertBuffer();

private:
    void buildInfo();
    QString selection();
    int indexFromPoint(QPoint p);
    void moveCursorTo(int destRow, int destCol);
    void selectWordAround(int row, int column);
    bool isConnected();
    int characterFromKeyPress(int key, Qt::KeyboardModifiers mod, bool *ok);
    void handleArrowKey(int key);
    void handleForwardDelete();
    inline QColor foregroundColorOf(BBS::CellAttribute &attr)
    {
        // NOTE: Get color from global preference considering bold
        return QColor("white");
    }
    inline int fColorIndex(BBS::CellAttribute &attribute)
    {
        if (attribute.f.reversed)
            return attribute.f.bColorIndex;
        else
            return attribute.f.fColorIndex;
    }
    inline int bColorIndex(BBS::CellAttribute &attribute)
    {
        if (attribute.f.reversed)
            return attribute.f.fColorIndex;
        else
            return attribute.f.bColorIndex;
    }
    inline int fBright(BBS::CellAttribute &attribute)
    {
        return !attribute.f.reversed && attribute.f.bright;
    }
    inline int bBright(BBS::CellAttribute &attribute)
    {
        return attribute.f.reversed && attribute.f.bright;
    }
    inline bool isAlphanumeric(uchar c)
    {
        return (std::isalnum(c) || (c == '-') || (c == '_') || (c == '.'));
    }
    inline bool isSpecialSymbol(ushort code)
    {
        switch (code)
        {
        case 0x2581:    // ▁ Lower one eighth block
        case 0x2582:    // ▂ Lower one quarter block
        case 0x2583:    // ▃ Lower three eighths block
        case 0x2584:    // ▄ Lower half block
        case 0x2585:    // ▅ Lower five eighths block
        case 0x2586:    // ▆ Lower three quarters block
        case 0x2587:    // ▇ Lower seven eights block
        case 0x2588:    // █ Full block
        case 0x2589:    // ▉ Left seven eights block
        case 0x258a:    // ▊ Left three quarters block
        case 0x258b:    // ▋ Left five eighths block
        case 0x258c:    // ▌ Left half block
        case 0x258d:    // ▍ Left three eighths block
        case 0x258e:    // ▎ Left one quarter block
        case 0x258f:    // ▏ Left one eighth block
        case 0x25e2:    // ◢ Black lower right triangle
        case 0x25e3:    // ◣ Black lower left triangle
        case 0x25e4:    // ◤ Black upper left triangle
        case 0x25e5:    // ◥ Black upper right triangle
        case 0x25fc:    // ◼ Black medium square
            return true;
        default:
            return false;
        }
        return false;
    }
    // NOTE: Need a preference object here
    double _cellWidth;
    double _cellHeight;
    int _row;
    int _column;
    int _x;
    int _y;
    QQueue<uchar> _insertBuffer;
    QTimer *_insertTimer;
    int _selectedStart;
    int _selectedLength;
    int _markedStart;
    int _markedLength;
    QPixmap *_backImage;
    bool _backImageFlipped;
    QVector<QSize> _singleAdvances;
    QVector<QSize> _doubleAdvances;
    Connection::Terminal *_terminal;
    QString _preeditString;
    bool _blinkOn;    // NOTE: Replace this with a global blink flag
    QPainter painter;

public: // Setters & Getters
    inline Connection::Terminal *terminal() const
    {
        return _terminal;
    }
    void setTerminal(Connection::Terminal *terminal);
};

}   // namespace Qelly

}   // namespace UJ

#endif // VIEW_H