#ifndef INGREDIENTFORM_H
#define INGREDIENTFORM_H

#include <QtWidgets>
#include "lineedit.h"

class IngredientForm : public QDialog
{
    Q_OBJECT
public:
    IngredientForm(QString, QWidget *, bool);

signals:

public slots:
};

#endif // INGREDIENTFORM_H
