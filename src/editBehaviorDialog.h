#ifndef EDIT_BEHAVIOR_DIALOG_INCLUDED__
#define EDIT_BEHAVIOR_DIALOG_INCLUDED__

#include <QDialog>
#include "ui_behaviorSettings.h"

class EditBehaviorDialog : public QDialog, public Ui_behaviorDialog {
public:
  inline EditBehaviorDialog(QWidget *parent = 0) : QDialog(parent) {setupUi(this);};
};

#endif
