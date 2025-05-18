#include "unitselectiondialog.h"

UnitSelectionDialog::UnitSelectionDialog(int totalBudget, QWidget *parent) :
    QDialog(parent), totalBudget(totalBudget), remainingBudget(totalBudget)
{
    setWindowTitle("Select Army Units");
    setStyleSheet(
        "QDialog { background-color: #f5f5f5; }"
        "QListWidget { background-color: white; border: 1px solid #ccc; }"
        "QLabel { font-weight: bold; color: #333; }"
        "QComboBox { padding: 3px; }"
        );

    unitCosts = {
        {"Heavy Infantry", 20},
        {"Light Infantry", 10},
        {"Archer", 15},
        {"Doctor", 25},
        {"Wizard", 30},
        {"Gulyay-Gorod", 40}
    };

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Select Your Units", this);
    titleLabel->setStyleSheet("QLabel { font-size: 16px; color: #0066cc; font-weight: bold; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    budgetLabel = new QLabel(QString("Remaining budget: %1/%2 gold").arg(remainingBudget).arg(totalBudget), this);
    budgetLabel->setStyleSheet("QLabel { font-size: 14px; }");
    mainLayout->addWidget(budgetLabel);

    unitTypeComboBox = new QComboBox(this);
    for (auto it = unitCosts.constBegin(); it != unitCosts.constEnd(); ++it) {
        unitTypeComboBox->addItem(QString("%1 (%2 gold)").arg(it.key()).arg(it.value()), it.key());
    }
    mainLayout->addWidget(unitTypeComboBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addUnitButton = new QPushButton("Add Unit", this);
    removeUnitButton = new QPushButton("Remove Selected", this);

    addUnitButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 5px; }"
        "QPushButton:hover { background-color: #45a049; }"
        );

    removeUnitButton->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; padding: 5px; }"
        "QPushButton:hover { background-color: #d32f2f; }"
        );

    buttonLayout->addWidget(addUnitButton);
    buttonLayout->addWidget(removeUnitButton);
    mainLayout->addLayout(buttonLayout);

    selectedUnitsList = new QListWidget(this);
    selectedUnitsList->setStyleSheet(
        "QListWidget { font-size: 12px; }"
        "QListWidget::item { padding: 5px; }"
        );
    mainLayout->addWidget(selectedUnitsList);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this
        );

    buttonBox->setStyleSheet(
        "QPushButton { min-width: 80px; padding: 5px; }"
        "QPushButton:default { background-color: #2196F3; color: white; }"
        );

    mainLayout->addWidget(buttonBox);

    connect(addUnitButton, &QPushButton::clicked, this, &UnitSelectionDialog::addUnit);
    connect(removeUnitButton, &QPushButton::clicked, this, &UnitSelectionDialog::removeUnit);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updateUI();
    resize(400, 500);
}


void UnitSelectionDialog::addUnit() {
    QString unitType = unitTypeComboBox->currentData().toString();
    int cost = unitCosts[unitType];

    if (remainingBudget >= cost) {
        selectedUnitsList->addItem(unitType);
        remainingBudget -= cost;
        updateUI();
    } else {
        QMessageBox::warning(this, "Not enough gold", "You don't have enough gold for this unit!");
    }
}

void UnitSelectionDialog::removeUnit() {
    QListWidgetItem *item = selectedUnitsList->currentItem();
    if (item) {
        QString unitType = item->text();
        remainingBudget += unitCosts[unitType];
        delete item;
        updateUI();
    }
}

void UnitSelectionDialog::updateUI() {
    budgetLabel->setText(QString("Remaining budget: %1/%2").arg(remainingBudget).arg(totalBudget));

    QDialogButtonBox *buttonBox = findChild<QDialogButtonBox*>();
    if (buttonBox) {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(selectedUnitsList->count() > 0);
    }
}

QVector<QString> UnitSelectionDialog::getSelectedUnits() const {
    QVector<QString> units;
    for (int i = 0; i < selectedUnitsList->count(); ++i) {
        units.append(selectedUnitsList->item(i)->text());
    }
    return units;
}

int UnitSelectionDialog::getRemainingBudget() const {
    return remainingBudget;
}

UnitSelectionDialog::~UnitSelectionDialog() {}
