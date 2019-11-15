#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    setWindowTitle("Products in details");
    setWindowIcon(QIcon(":/icon.png"));
    model_builder = new ModelBuilder(file_path);
    ui->treeView->setModel(model_builder->GetTreeModel());
    ui->listView->setModel(model_builder->GetListModel());
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();

    connect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
         this, SLOT(selectTreeItem(const QItemSelection&,const QItemSelection&)));
    connect(ui->listView->selectionModel(),SIGNAL(currentChanged(const QModelIndex, const QModelIndex)),
         this, SLOT(selectListItem(const QModelIndex, const QModelIndex)));
}

Widget::~Widget()
{
    delete model_builder;
}

void Widget::selectTreeItem(const QItemSelection &selected,const QItemSelection &deselected)
{
    stop_selection = !stop_selection;
    if(stop_selection == false) return;

    if (!ui->treeView->selectionModel()->selection().isEmpty())
    {
        ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());
        QModelIndex index = model_builder->tree_to_list[selected.indexes().at(0)];
        ui->listView->setCurrentIndex(index);
        ui->tableView->setModel(model_builder->GetTabModel(index));
        ui->tableView->setSpan(0, 0, 1, 2);
    }
}

void Widget::selectListItem(const QModelIndex selected, const QModelIndex deselected)
{
    stop_selection = !stop_selection;
    if(stop_selection == false) return;
    QModelIndex list_item_index = ui->listView->model()->index(selected.row(), 0);

    QModelIndex index = model_builder->list_to_tree[list_item_index].tree_item;
    ui->treeView->setCurrentIndex(index);
    ui->tableView->setModel(model_builder->GetTabModel(list_item_index));
    ui->tableView->setSpan(0, 0, 1, 2);
}
