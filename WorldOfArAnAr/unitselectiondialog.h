#ifndef UNITSELECTIONDIALOG_H
#define UNITSELECTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

class UnitSelectionDialog : public QDialog {
    Q_OBJECT
public:
    explicit UnitSelectionDialog(int totalBudget = 200, QWidget *parent = nullptr);
    ~UnitSelectionDialog();
    QVector<QString> getSelectedUnits() const;
    int getRemainingBudget() const;

private slots:
    void addUnit();
    void removeUnit();
    void updateUI();

private:
    QComboBox *unitTypeComboBox;
    QListWidget *selectedUnitsList;
    QLabel *budgetLabel;
    QPushButton *addUnitButton;
    QPushButton *removeUnitButton;

    int totalBudget;
    int remainingBudget;
    QMap<QString, int> unitCosts;
};


#endif // UNITSELECTIONDIALOG_H
