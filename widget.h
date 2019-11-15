#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include "modelbuilder.h"

namespace Ui { class Widget; }

class Widget : public QWidget {
    Q_OBJECT
    Ui::Widget *ui;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void selectTreeItem(const QItemSelection&, const QItemSelection&);
    void selectListItem(const QModelIndex, const QModelIndex);

private:
    const QString file_path = ":/default.xml";
    ModelBuilder* model_builder;
    bool stop_selection = false;
};
#endif // WIDGET_H
