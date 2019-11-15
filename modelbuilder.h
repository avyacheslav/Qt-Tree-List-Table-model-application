#ifndef MODELBUILDER_H
#define MODELBUILDER_H
#include <QtXml>
#include <QStandardItemModel>

struct StringPair
{
    QString name;
    QString value;
};

struct ListItemData
{
    QModelIndex tree_item;
    QList<StringPair> list_item_data;
};

class ModelBuilder
{
public:
    ModelBuilder(QString file_path);
    ~ModelBuilder();

    QStandardItemModel* GetTreeModel();
    QStringListModel* GetListModel();
    QStandardItemModel* GetTabModel(QModelIndex index);
    void SetTabInfo();

    QHash<QModelIndex, QModelIndex> tree_to_list;
    QHash<QModelIndex, ListItemData> list_to_tree;

private:
    void GetXmlData(QString xml_file_path);
    void GetElementData(QStandardItem* treeitem, QDomNode &dom_node, QHash<QString, QDomNode> &hash, bool from_hash = false);
    QStandardItem* SetChild(QStandardItem* parent, QString name);

    QStandardItemModel* tree_model;
    QStringListModel* list_model;
    QStandardItemModel* tab_model;
};

#endif // MODELBUILDER_H
