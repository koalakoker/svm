#ifndef AXISPROPERTIES_H
#define AXISPROPERTIES_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

#include "axis.h"

class AxisPropierties : public QObject
{
    Q_OBJECT
public:
    explicit AxisPropierties(QObject *parent = nullptr);
    Axis* axis;

public slots:
    void set(void);

signals:
    void axisUpdated(void);

private:
    QDialog m_diag;
    QLineEdit leXmin;
    QCheckBox cbXdiv;
    QLabel lXdiv;
    QLineEdit leXdiv;
    QLineEdit leXmax;
    QLineEdit leYmin;
    QCheckBox cbYdiv;
    QLabel lYdiv;
    QLineEdit leYdiv;
    QLineEdit leYmax;
    enum {
        SHOW_NONE,
        SHOW_BOTTOM_LEFT,
        SHOW_TOP_RIGHT
    };
    QComboBox cbLabelsPosBox;

    void createDialog(void);
    void updateDialog(void);
    void backupState(void);
    void restoreState(void);
    void updateParentLabelPos(void);

    // State for backup
    QRectF m_range;
    QPointF m_div;
    bool m_divVisible[2];
    bool m_showBottom;
    bool m_showTop;
    bool m_showLeft;
    bool m_showRight;

private slots:
    void onOk(void);
    void onClose(void);
    void rejected(void);
    void updateParent(void);
    void updateControlsVisibility(void);
    void cbChanged(int state);
    void cbLabelsPosBoxChanged(int index);
};

#endif // AXISPROPERTIES_H
