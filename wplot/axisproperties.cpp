#include "axisproperties.h"
#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>

AxisPropierties::AxisPropierties(QObject *parent)
    : QObject{parent} {
    createDialog();
}
void AxisPropierties::set(void) {
    backupState();
    updateDialog();
    updateControlsVisibility();
    m_diag.show();
    connect(&cbXdiv, SIGNAL(stateChanged(int)), this, SLOT(cbChanged(int)));
    connect(&cbYdiv, SIGNAL(stateChanged(int)), this, SLOT(cbChanged(int)));
    connect(&cbLabelsPosBox, SIGNAL(currentIndexChanged(int)), this, SLOT(cbLabelsPosBoxChanged(int)));
}
void AxisPropierties::onOk(void) {
    updateParent();
    m_diag.accept();
}
void AxisPropierties::onClose(void) {
    m_diag.reject();
}
void AxisPropierties::rejected(void) {
    restoreState();
    emit axisUpdated();
}
void AxisPropierties::createDialog(void) {
    QVBoxLayout *layMain = new QVBoxLayout;

    // Inputs
    QHBoxLayout *layInputs = new QHBoxLayout;

    // Sx
    QVBoxLayout *layInputsSx = new QVBoxLayout;
    QLabel* lXmin = new QLabel("x min:");
    cbXdiv.setText("Enable x divisions");
    lXdiv.setText("x divisions:");
    QLabel* lXmax = new QLabel("x max:");
    layInputsSx->addWidget(lXmin);
    layInputsSx->addWidget(&leXmin);
    layInputsSx->addWidget(lXmax);
    layInputsSx->addWidget(&leXmax);
    layInputsSx->addWidget(&cbXdiv);
    layInputsSx->addWidget(&lXdiv);
    layInputsSx->addWidget(&leXdiv);
    layInputsSx->addStretch();

    connect(&leXmin, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leXdiv, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leXmax, SIGNAL(editingFinished()), this, SLOT(updateParent()));

    // Dx
    QVBoxLayout *layInputsDx = new QVBoxLayout;
    QLabel* lYmin = new QLabel("y min:");
    cbYdiv.setText("Enable y divisions");
    lYdiv.setText("y divisions:");
    QLabel* lYmax = new QLabel("y max:");
    layInputsDx->addWidget(lYmin);
    layInputsDx->addWidget(&leYmin);
    layInputsDx->addWidget(lYmax);
    layInputsDx->addWidget(&leYmax);
    layInputsDx->addWidget(&cbYdiv);
    layInputsDx->addWidget(&lYdiv);
    layInputsDx->addWidget(&leYdiv);
    layInputsDx->addStretch();

    connect(&leYmin, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leYdiv, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leYmax, SIGNAL(editingFinished()), this, SLOT(updateParent()));

    layInputs->addLayout(layInputsSx);
    layInputs->addLayout(layInputsDx);

    // Axis label show
    cbLabelsPosBox.addItem("Axis label none", QVariant(SHOW_NONE));
    cbLabelsPosBox.addItem("Axis label bottom left", QVariant(SHOW_BOTTOM_LEFT));
    cbLabelsPosBox.addItem("Axis label top right", QVariant(SHOW_TOP_RIGHT));

    // Buttons
    QHBoxLayout *layButtons = new QHBoxLayout;
    QAbstractButton *bOk = new QPushButton("Ok");
    QAbstractButton *bCancel = new QPushButton("Cancel");
    layButtons->addWidget(bOk);
    layButtons->addWidget(bCancel);
    m_diag.connect(bOk, SIGNAL(clicked()), this, SLOT(onOk()));
    m_diag.connect(bCancel, SIGNAL(clicked()), this, SLOT(onClose()));

    layMain->addLayout(layInputs);
    layMain->addWidget(&cbLabelsPosBox);
    layMain->addLayout(layButtons);

    m_diag.setWindowTitle("Axis properties");
    m_diag.setLayout(layMain);
    connect(&m_diag, SIGNAL(rejected()), this, SLOT(rejected()));
}
void AxisPropierties::updateDialog(void) {
    leXmin.setText(QString::number(axis->m_range.left()));
    if (axis->m_divVisible[0]) {
        cbXdiv.setCheckState(Qt::Checked);
    } else {
        cbXdiv.setCheckState(Qt::Unchecked);
    }
    leXdiv.setText(QString::number(axis->m_div.x()));
    leXmax.setText(QString::number(axis->m_range.right()));
    leYmin.setText(QString::number(axis->m_range.top()));
    if (axis->m_divVisible[1]) {
        cbYdiv.setCheckState(Qt::Checked);
    } else {
        cbYdiv.setCheckState(Qt::Unchecked);
    }
    leYdiv.setText(QString::number(axis->m_div.y()));
    leYmax.setText(QString::number(axis->m_range.bottom()));

    if ((axis->m_showBottom) || (axis->m_showLeft)) {
        cbLabelsPosBox.setCurrentIndex(SHOW_BOTTOM_LEFT);
    } else if ((axis->m_showTop) || (axis->m_showRight)) {
        cbLabelsPosBox.setCurrentIndex(SHOW_TOP_RIGHT);
    } else {
        cbLabelsPosBox.setCurrentIndex(SHOW_NONE);
    }
}
void AxisPropierties::updateParent(void) {
    axis->m_range.setLeft  (leXmin.text().toDouble());
    axis->m_range.setRight (leXmax.text().toDouble());
    axis->m_range.setTop   (leYmin.text().toDouble());
    axis->m_range.setBottom(leYmax.text().toDouble());
    axis->m_div.setX(leXdiv.text().toDouble());
    axis->m_div.setY(leYdiv.text().toDouble());
    axis->m_divVisible[0] = (cbXdiv.checkState() == Qt::Checked);
    axis->m_divVisible[1] = (cbYdiv.checkState() == Qt::Checked);
    updateParentLabelPos();
    emit axisUpdated();
}
void AxisPropierties::updateParentLabelPos(void) {
    int index = cbLabelsPosBox.currentIndex();
    switch (index) {
        default:
        case SHOW_NONE:
        {
            axis->m_showBottom = false;
            axis->m_showTop = false;
            axis->m_showLeft = false;
            axis->m_showRight = false;
            break;
        }
        case SHOW_BOTTOM_LEFT:
        {
            axis->m_showBottom = true;
            axis->m_showTop = false;
            axis->m_showLeft = true;
            axis->m_showRight = false;
            break;
        }
        case SHOW_TOP_RIGHT:
        {
            axis->m_showBottom = false;
            axis->m_showTop = true;
            axis->m_showLeft = false;
            axis->m_showRight = true;
            break;
        }
    }
    emit axisUpdated();
}
void AxisPropierties::backupState(void) {
    m_range = axis->m_range;
    m_div = axis->m_div;
    m_divVisible[0] = axis->m_divVisible[0];
    m_divVisible[1] = axis->m_divVisible[1];
    m_showBottom = axis->m_showBottom;
    m_showTop = axis->m_showTop;
    m_showLeft = axis->m_showLeft;
    m_showRight = axis->m_showRight;
}
void AxisPropierties::restoreState(void) {
    axis->m_range = m_range;
    axis->m_div = m_div;
    axis->m_divVisible[0] = m_divVisible[0];
    axis->m_divVisible[1] = m_divVisible[1];
    axis->m_showBottom = m_showBottom;
    axis->m_showTop = m_showTop;
    axis->m_showLeft = m_showLeft;
    axis->m_showRight = m_showRight;
}
void AxisPropierties::updateControlsVisibility(void) {
    lXdiv.setVisible(axis->m_divVisible[0]);
    leXdiv.setVisible(axis->m_divVisible[0]);
    lYdiv.setVisible(axis->m_divVisible[1]);
    leYdiv.setVisible(axis->m_divVisible[1]);
}
void AxisPropierties::cbChanged(int state) {
    Q_UNUSED(state);
    updateParent();
    updateControlsVisibility();
}
void AxisPropierties::cbLabelsPosBoxChanged(int index) {
    Q_UNUSED(index);
    updateParent();
}
