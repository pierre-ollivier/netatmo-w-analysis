#include "EphemerisPanel.h"

EphemerisPanel::EphemerisPanel() : QGroupBox()
{
    layout = new QGridLayout();

    setTitle("Statistiques pour un _ _");
    setLayout(layout);
    setAlignment(Qt::AlignHCenter);
}

void EphemerisPanel::setDate(QDate date) {
    setTitle("Statistiques pour un " + date.toString("dd MMMM"));
}
