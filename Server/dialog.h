#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <QDialog>

class QListWidgetItem;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void updateRoomList();

    void output(const char *format, ...);

private slots:
    void on_list_room_itemClicked(QListWidgetItem *item);

private:
    Ui::Dialog *ui;
};

extern Dialog *g_dialog;

#endif // DIALOG_H
