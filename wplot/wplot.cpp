#include "wplot/wplot.h"
#include "wplot/axis.h"
#include "wplot/cursor.h"
#include "wplot/curve.h"
#include "wplot/jsonserial.h"

#include <QGestureEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QFileDialog>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QLabel>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>

WPlot::WPlot(QWidget *parent, Curve::style_t defaultStyle) :
    QWidget(parent), m_plotter(NULL), m_defaultStyle(defaultStyle)
{
    setFocusPolicy(Qt::StrongFocus);
    this->wParent = parent;

    grabGesture(Qt::PinchGesture);
    setMouseTracking(true);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenu(QPoint)));

    this->state = &this->normalState;
    this->state->setCursor(*this);
}
WPlot::~WPlot()
{
}

// Add point
void WPlot::addPoint(double t, SData y)
{
    foreach (double val, y.data())
    {
        if (val > m_y_max)
        {
            m_y_max = val;
        }

        if (val < m_y_min)
        {
            m_y_min = val;
        }

    }

    y.prepend(t);
    m_data.append(y);
}

// Create plot
void WPlot::createPlot(void)
{
    if (m_data.size() != 0)
    {
        m_x_min = m_data[0][0];
        m_x_max = m_data[m_data.size()-1][0];
    }

    QSize plotterSize;
    if (wParent == NULL) {
        plotterSize = this->size();
    } else {
        plotterSize = this->wParent->size();
        this->resize(plotterSize);
    }

    m_plotter = new Plotter(
                plotterSize,
                QRectF(m_x_min, m_y_min, m_x_max - m_x_min, m_y_max - m_y_min),
                m_defaultStyle);
    m_plotter->curve->m_data = m_data;
    m_axisProps.axis = m_plotter->axis;
    emit newPlotter();
    updatePlot();
}

// Slots
void WPlot::updatePlot(void) {
    m_plotImage = m_plotter->plot();
    repaint();
}
void WPlot::zoom_Undo(void)
{
    if (!m_plotter)
        return;
    m_plotter->axis->zoom->undo();
    updatePlot();
}
void WPlot::zoom_Redo(void)
{
    if (!m_plotter)
        return;
    m_plotter->axis->zoom->redo();
    updatePlot();
}
void WPlot::zoom(void)
{
    this->state = &this->zoomState;
    this->state->setCursor(*this);
}
void WPlot::hZoom(void)
{
    this->state = &this->hZoomState;
    this->state->setCursor(*this);
}
void WPlot::vZoom(void)
{
    this->state = &this->vZoomState;
    this->state->setCursor(*this);
}
void WPlot::fullHZoom(void) {
    m_plotter->axis->zoom->addUndoStatus();
    m_plotter->axis->zoom->xToFullRange();
    updatePlot();
}
void WPlot::fullVZoom(void) {
    m_plotter->axis->zoom->addUndoStatus();
    m_plotter->axis->zoom->yToFullRange();
    updatePlot();
}
void WPlot::loadData(void)
{
    QFileDialog* diag = new QFileDialog(this,"OPen data file","","Text files (*.txt);;Data files (*.dat);;All files (*.*)");
    connect(diag,SIGNAL(fileSelected(QString)), this, SLOT(loadData(QString)));
    diag->show();
}
void WPlot::loadData(QString fileName, bool update)
{
    double y_max = 0, y_min = 0;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        m_data.clear();
        QTextStream stream(&file);
        QString line;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            QStringList fields = line.split(QRegularExpression("[\t ]+"));
            SData sample;
            for (int i = 0; i < fields.size(); i++)
            {
                double val = fields.at(i).toDouble();
                sample.append(val);
                if (i != 0)
                {
                    if (val > y_max)
                    {
                        y_max = val;
                    }

                    if (val < y_min)
                    {
                        y_min = val;
                    }
                }
            }
            m_data.append(sample);
        }
        m_fileName = fileName;

        if (m_data.size() != 0)
        {
            m_x_min = m_data[0][0];
            m_x_max = m_data[m_data.size()-1][0];
        }

        if (m_plotter)
        {
            delete m_plotter; // remove old data plotter
        }

        m_plotter = new Plotter(
                    size(),
                    QRectF(m_x_min, y_min, m_x_max - m_x_min, y_max - y_min),
                    m_defaultStyle);
        m_plotter->curve->m_data = m_data;
        m_axisProps.axis = m_plotter->axis;
        emit newPlotter();
        if (update) {
            updatePlot();
        }
    }
    file.close();
}
void WPlot::saveData(void)
{
    QFileDialog* diag = new QFileDialog(this,"Export data file","","Text files (*.txt);;Data files (*.dat);;All files (*.*)");
    connect(diag,SIGNAL(fileSelected(QString)), this, SLOT(saveData(QString)));
    diag->setAcceptMode(QFileDialog::AcceptSave);
    diag->show();
}
void WPlot::saveData(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for (int row = 0; row < m_data.size(); row++)
        {
            SData dataRow = m_data.at(row);
            for (int i = 0; i < dataRow.size(); i++)
            {
                double val = dataRow.data().at(i);
                if (i == 0)
                {
                    stream << val;
                }
                else
                {
                    stream << " " << val;
                }
            }
            stream << Qt::endl;
        }
    }
    file.close();
}
void WPlot::saveSettings(void) {
    QFileDialog* diag = new QFileDialog(this,"Export setting file","","JSON files (*.json);;All files (*.*)");
    connect(diag,SIGNAL(fileSelected(QString)), this, SLOT(saveSettings(QString)));
    diag->setAcceptMode(QFileDialog::AcceptSave);
    diag->show();
}
void WPlot::loadSettings(void) {
    QFileDialog* diag = new QFileDialog(this,"Open setting file","","JSON files (*.json);;All files (*.*)");
    connect(diag,SIGNAL(fileSelected(QString)), this, SLOT(loadSettings(QString)));
    diag->show();
}
void WPlot::saveSettings(QString fileName) {
    JSONSerial js;
    js.add("Xmin", m_plotter->axis->m_range.left());
    js.add("Xmax", m_plotter->axis->m_range.right());
    js.add("Ymin", m_plotter->axis->m_range.bottom());
    js.add("Ymax", m_plotter->axis->m_range.top());
    js.add("Xdiv", m_plotter->axis->m_div.x());
    js.add("Ydiv", m_plotter->axis->m_div.y());
    js.add("XdivVisible", m_plotter->axis->m_divVisible[0]);
    js.add("YdivVisible", m_plotter->axis->m_divVisible[1]);
    js.add("showBottom", m_plotter->axis->m_showBottom);
    js.add("showLeft", m_plotter->axis->m_showLeft);
    js.add("showTop", m_plotter->axis->m_showTop);
    js.add("showRight", m_plotter->axis->m_showRight);
    js.save(fileName);
}
void WPlot::loadSettings(QString fileName, bool update) {
    JSONSerial js;
    js.load(fileName);
    double value;
    js.read("Xmin", value);
    m_plotter->axis->m_range.setLeft(value);
    js.read("Xmax", value);
    m_plotter->axis->m_range.setRight(value);
    js.read("Ymin", value);
    m_plotter->axis->m_range.setBottom(value);
    js.read("Ymax", value);
    m_plotter->axis->m_range.setTop(value);
    js.read("Xdiv", value);
    m_plotter->axis->m_div.setX(value);
    js.read("Ydiv", value);
    m_plotter->axis->m_div.setY(value);
    js.read("XdivVisible", m_plotter->axis->m_divVisible[0]);
    js.read("YdivVisible", m_plotter->axis->m_divVisible[1]);
    js.read("showBottom", m_plotter->axis->m_showBottom);
    js.read("showLeft", m_plotter->axis->m_showLeft);
    js.read("showTop", m_plotter->axis->m_showTop);
    js.read("showRight", m_plotter->axis->m_showRight);
    if (update) {
        updatePlot();
    }
}
void WPlot::saveFigure(void) {
    QFileDialog* diag = new QFileDialog(this,"Save figure","","Figure files (*.fig);;All files (*.*)");
    connect(diag,SIGNAL(fileSelected(QString)), this, SLOT(saveFigure(QString)));
    diag->setAcceptMode(QFileDialog::AcceptSave);
    diag->show();
}
void WPlot::loadFigure(void) {
    QFileDialog* diag = new QFileDialog(this,"Load figure","","Figure files (*.fig);;All files (*.*)");
    connect(diag,SIGNAL(fileSelected(QString)), this, SLOT(loadFigure(QString)));
    diag->show();
}
void WPlot::saveFigure(QString fileName) {
    QFileInfo info(fileName);
    QString dataFileName     = info.path() + "/" + info.completeBaseName() + ".dat";
    QString settingsFileName = info.path() + "/" + info.completeBaseName() + ".json";
    saveData(dataFileName);
    saveSettings(settingsFileName);
    JSONSerial js;
    js.add("DataFilePath", dataFileName);
    js.add("SettingsFilePath", settingsFileName);
    js.save(fileName);
}
void WPlot::loadFigure(QString fileName) {
    JSONSerial js;
    js.load(fileName);
    QString dataFileName;
    QString settingsFileName;
    js.read("DataFilePath", dataFileName);
    loadData(dataFileName, false);
    js.read("SettingsFilePath", settingsFileName);
    loadSettings(settingsFileName, false);
    updatePlot();
}
// Context Menu
void WPlot::ShowContextMenu(QPoint pos)
{
    m_pos = pos;
    bool onCursor = m_plotter->cursor->on(pos, this->selectedCursor, false);

    QMenu contextMenu("Context menu", this);

    // Cursor Menu
    QMenu cursorMenu("Cursor", this);

    QAction addCursorAction("Add cursor", this);
    connect(&addCursorAction, SIGNAL(triggered()), this, SLOT(addCursor()));
    cursorMenu.addAction(&addCursorAction);

    QAction removeCursorAction("Remove cursor", this);
    QAction setCursorPosAction("Set cursor", this);
    if (onCursor) {
        connect(&removeCursorAction, SIGNAL(triggered()), this, SLOT(removeCursor()));
        cursorMenu.addAction(&removeCursorAction);

        connect(&setCursorPosAction, SIGNAL(triggered()), this, SLOT(setCursorPos()));
        cursorMenu.addAction(&setCursorPosAction);
    }

    contextMenu.addMenu(&cursorMenu);

    // Zoom Menu
    QMenu zoomMenu("Zoom", this);

    QAction zoomAction("Zoom", this);
    connect(&zoomAction, SIGNAL(triggered()), this, SLOT(zoom()));
    zoomMenu.addAction(&zoomAction);

    QAction hZoomAction("Horizontal zoom", this);
    connect(&hZoomAction, SIGNAL(triggered()), this, SLOT(hZoom()));
    zoomMenu.addAction(&hZoomAction);

    QAction vZoomAction("Vertical zoom", this);
    connect(&vZoomAction, SIGNAL(triggered()), this, SLOT(vZoom()));
    zoomMenu.addAction(&vZoomAction);

    zoomMenu.addSeparator();

    QAction zoomUndoAction("Zoom Undo", this);
    connect(&zoomUndoAction, SIGNAL(triggered()), this, SLOT(zoom_Undo()));
    zoomMenu.addAction(&zoomUndoAction);

    QAction zoomRedoAction("Zoom Redo", this);
    connect(&zoomRedoAction, SIGNAL(triggered()), this, SLOT(zoom_Redo()));
    zoomMenu.addAction(&zoomRedoAction);

    zoomMenu.addSeparator();

    QAction zoomFullHorizontalAction("Full hor. range", this);
    connect(&zoomFullHorizontalAction, SIGNAL(triggered()), this,SLOT(fullHZoom()));
    zoomMenu.addAction(&zoomFullHorizontalAction);

    QAction zoomFullVerticalAction("Full ver. range", this);
    connect(&zoomFullVerticalAction, SIGNAL(triggered()), this,SLOT(fullVZoom()));
    zoomMenu.addAction(&zoomFullVerticalAction);

    contextMenu.addMenu(&zoomMenu);

    // Save/Load Menu
    QMenu saveLoadMenu("Save/Load", this);

    QAction exportFigureAction("Save figure", this);
    connect(&exportFigureAction, SIGNAL(triggered()), this, SLOT(saveFigure()));
    saveLoadMenu.addAction(&exportFigureAction);

    QAction openFigureAction("Load figure", this);
    connect(&openFigureAction, SIGNAL(triggered()), this, SLOT(loadFigure()));
    saveLoadMenu.addAction(&openFigureAction);

    saveLoadMenu.addSeparator();

    QAction exportDataAction("Save data", this);
    connect(&exportDataAction, SIGNAL(triggered()), this, SLOT(saveData()));
    saveLoadMenu.addAction(&exportDataAction);

    QAction openDataAction("Load data", this);
    connect(&openDataAction, SIGNAL(triggered()), this, SLOT(loadData()));
    saveLoadMenu.addAction(&openDataAction);

    saveLoadMenu.addSeparator();

    QAction saveSettingsAction("Save settings", this);
    connect(&saveSettingsAction, SIGNAL(triggered()), this, SLOT(saveSettings()));
    saveLoadMenu.addAction(&saveSettingsAction);

    QAction loadSettingsAction("Load settings", this);
    connect(&loadSettingsAction, SIGNAL(triggered()), this, SLOT(loadSettings()));
    saveLoadMenu.addAction(&loadSettingsAction);

    contextMenu.addMenu(&saveLoadMenu);

    // Axis menu
    QMenu axisMenu("Axis", this);

    QAction setAxisAction("Axis properties", this);
    connect(&setAxisAction, SIGNAL(triggered()), &this->m_axisProps, SLOT(set()));
    connect(&this->m_axisProps, SIGNAL(axisUpdated()), this, SLOT(updatePlot()));
    axisMenu.addAction(&setAxisAction);

    contextMenu.addMenu(&axisMenu);

    contextMenu.exec(mapToGlobal(pos));
}

// Cursor functions
void WPlot::addCursor(void) {
    m_plotter->cursor->addAtPixel(m_pos.x());
    updatePlot();
}
void WPlot::removeCursor(void) {
    m_plotter->cursor->remove(this->selectedCursor);
    updatePlot();
}
void WPlot::setCursorPos(void) {
    this->setCurPosDiag = new QDialog();
    QVBoxLayout *lay = new QVBoxLayout;
    QLabel *label = new QLabel("Insert x position:");
    this->curPosEdit = new QLineEdit();
    QAbstractButton *bOk = new QPushButton("Ok");
    QAbstractButton *bCancel = new QPushButton("Cancel");

    QHBoxLayout *buttonsLay = new QHBoxLayout;
    buttonsLay->addWidget(bOk);
    buttonsLay->addWidget(bCancel);

    lay->addWidget(label);
    lay->addWidget(this->curPosEdit);
    lay->addLayout(buttonsLay);

    this->setCurPosDiag->setLayout(lay);
    this->setCurPosDiag->show();
    this->setCurPosDiag->connect(bOk,SIGNAL(clicked()),this,SLOT(cursorNewPos()));
    this->setCurPosDiag->connect(bCancel,SIGNAL(clicked()),this->setCurPosDiag,SLOT(close()));
}
void WPlot::cursorNewPos(void) {
    QString inputText = curPosEdit->text();
    bool isValid;
    qreal value = inputText.toDouble(&isValid);
    if (isValid) {
        m_plotter->cursor->setPos(selectedCursor, value);
        updatePlot();
    }
    this->setCurPosDiag->close();
}

// Events
void WPlot::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(QPoint(0, 0), m_plotImage);
}
void WPlot::keyPressEvent(QKeyEvent* event) {
    this->state->keyPressEvent(*this, event);
    if (!event->isAccepted()) {
        QWidget::keyPressEvent(event);
    }
};
void WPlot::keyReleaseEvent(QKeyEvent* event) {
    this->state->keyReleaseEvent(*this, event);
    QWidget::keyReleaseEvent(event);
};
void WPlot::mousePressEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    this->state->mousePressEvent(*this, event);
}
void WPlot::mouseDoubleClickEvent(QMouseEvent* event)
{
    this->state->mouseDoubleClickEvent(*this, event);
}
void WPlot::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    this->state->mouseReleaseEvent(*this, event);
}
void WPlot::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    this->state->mouseMoveEvent(*this, event);
}
void WPlot::wheelEvent(QWheelEvent* event)
{
    if (!m_plotter)
        return;
    QPoint angleDelta = event->angleDelta();
    if (angleDelta != QPoint(0,0))
    {
        m_plotter->axis->zoom->addUndoStatus(); // To be filetered
        m_plotter->axis->zoom->x((qreal)angleDelta.y()/120);
        m_plotter->axis->zoom->y((qreal)angleDelta.x()/120);
        updatePlot();
    }
}
bool WPlot::event(QEvent *event)
{
    if (!m_plotter)
        QWidget::event(event);
    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent* gest = static_cast<QGestureEvent*>(event);
        if (QGesture *pinch = gest->gesture(Qt::PinchGesture))
        {
            QPinchGesture* pinchGest = static_cast<QPinchGesture *>(pinch);
            qreal factor = pinchGest->scaleFactor();
            m_plotter->axis->zoom->addUndoStatus(); // To be filetered
            m_plotter->axis->zoom->x((factor-1)*5);
            m_plotter->axis->zoom->y((factor-1)*5);
            updatePlot();
        }
    }
    return QWidget::event(event);
}
void WPlot::resizeEvent(QResizeEvent *event)
{
    if (!m_plotter)
        return;
    if (event->type() == QEvent::Resize)
    {
        m_plotter->setSize(size());
        updatePlot();
    }
}
