#include "wcursorinfo.h"
#include "ui_wcursorinfo.h"

#include <QDebug>

WCursorInfo::WCursorInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WCursorInfo)
{
    ui->setupUi(this);
}

WCursorInfo::~WCursorInfo()
{
    delete ui;
}

/* Copy pasted from the WEB */
static QSize myGetQTableWidgetSize(QTableWidget *t) {
   int w = t->verticalHeader()->width() + 4; // +4 seems to be needed
   for (int i = 0; i < t->columnCount(); i++)
      w += t->columnWidth(i); // seems to include gridline (on my machine)
   int h = t->horizontalHeader()->height() + 4;
   for (int i = 0; i < t->rowCount(); i++)
      h += t->rowHeight(i);
   return QSize(w, h);
}

void WCursorInfo::updateInfo(QVector<QVector<double>> cursorInfo)
{
    const Qt::GlobalColor plotColor[] = {Qt::darkGray, Qt::black, Qt::blue, Qt::green, Qt::red};
    int cursorNum = cursorInfo.size();
    if (cursorNum > 0)
    {
        int trackNum = cursorInfo[0].size();
        ui->tableWidget->setColumnCount(trackNum);

        QStringList horizontalLabels;
        horizontalLabels << "x";
        for (int i = 0; i < trackNum; i++)
        {
            horizontalLabels << QString("y%1").arg(i+1);
        }
        ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);

        ui->tableWidget->setRowCount(cursorNum);
        for (int cur = 0; cur < cursorNum; cur++)
        {
            for (int track = 0;  track < trackNum;  track++)
            {
                QTableWidgetItem* cell = new QTableWidgetItem(QString::number(cursorInfo[cur][track]));
                cell->setForeground(plotColor[track]);
                if (track == 0)
                {

                }
                else
                {
                    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
                }
                ui->tableWidget->blockSignals(true);
                ui->tableWidget->setItem(cur,track,cell);
                ui->tableWidget->blockSignals(false);
            }
        }
    }
    else
    {
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setRowCount(0);
        QStringList horizontalLabels;
        horizontalLabels << "x" << "y";
        ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);
    }
    QSize tabSize = myGetQTableWidgetSize(ui->tableWidget);
    QSize layoutSize(5,5);
    resize(tabSize+layoutSize);
}

void WCursorInfo::on_tableWidget_cellChanged(int row, int column)
{
    emit cursorPosChanged(row, ui->tableWidget->item(row,column)->text().toDouble());
}
