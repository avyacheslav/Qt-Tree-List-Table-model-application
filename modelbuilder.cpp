#include "modelbuilder.h"
#include <QStandardItemModel>

ModelBuilder::ModelBuilder(QString file_path)
{
    tree_model = new QStandardItemModel();
    list_model = new QStringListModel();
    tab_model = new QStandardItemModel();
    GetXmlData(file_path);
}

ModelBuilder::~ModelBuilder()
{
    delete tree_model;
    delete list_model;
    delete tab_model;
}

QStandardItemModel* ModelBuilder::GetTreeModel()
{
    return  tree_model;
}

QStringListModel* ModelBuilder::GetListModel()
{
    return  list_model;
}

QStandardItemModel* ModelBuilder::GetTabModel(QModelIndex index)
{
    QList<StringPair> item_data = list_to_tree[index].list_item_data;
    tab_model->clear();
    tab_model->setItem(0, 0, new QStandardItem("Properties"));

    for(int i = 0; i < item_data.length(); i++)
    {
        StringPair pair = item_data.at(i);
        tab_model->setItem(i + 1, 0, new QStandardItem(pair.name));
        tab_model->setItem(i + 1, 1, new QStandardItem(pair.value));
    }

    return  tab_model;
}

void ModelBuilder::GetXmlData(QString xml_file_path)
{
    QFile xml_file(xml_file_path);
    if (!xml_file.exists() || !xml_file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QDomDocument dom_ocument;
    dom_ocument.setContent(&xml_file);
    xml_file.close();
    QDomElement top_element = dom_ocument.documentElement();
    if(top_element.isNull()) return;
    QDomNodeList node_list = top_element.childNodes();
    if(node_list.isEmpty()) return;
    QHash<QString, QDomNode> hash;

    for(int i = 0; i < node_list.length(); i++)
    {
        QString id = node_list.at(i).toElement().attribute("id");
        if(!id.isNull() && !id.isEmpty())
        {
            hash.insert(id, node_list.at(i));
        }
    }

    QDomNode dom_node = top_element.firstChild();

    while (!dom_node.isNull())
    {
        QDomElement dom_element = dom_node.toElement();

        if(dom_element.attribute("id") == QString(""))
        {
            GetElementData(nullptr, dom_element, hash);
        }

        dom_node = dom_node.nextSibling();
    }
}

void ModelBuilder::GetElementData(QStandardItem* tree_item, QDomNode &dom_node, QHash<QString, QDomNode> &hash, bool from_hash)
{
    QDomElement element = dom_node.toElement();
    QString name = element.attribute("name");
    QString id = element.attribute("id");
    QDomNode child = dom_node.firstChild();
    QDomNode node = hash[id];

    if(!name.isEmpty())
    {        
        QStandardItem* item = SetChild(tree_item, name.isEmpty() ? id : name);

        while(!child.isNull())
        {
            GetElementData(item, child, hash);
            child = child.nextSibling();
        }

        if(!id.isEmpty() && !node.isNull() && !from_hash)
        {
            GetElementData(item, node, hash, true);
        }
    }
    else if(!id.isEmpty())
    {
        while(!child.isNull())
        {
            GetElementData(tree_item, child, hash);
            child = child.nextSibling();
        }

        if(child.isNull() && name.isEmpty())
        {
            if(!node.isNull() && !from_hash)
            {
                GetElementData(tree_item, node, hash, true);
                return;
            }

            if(!from_hash)
            {
                SetChild(tree_item, name.isEmpty() ? id : name);
            }
        }

    }
    else
    {
        if(list_model->rowCount() == 0)
        {
            list_model->insertRow(0);
            QModelIndex index = list_model->index(0);
            list_model->setData(index, "Parts instances");
            list_model->insertRow(1);
        }

        if(element.tagName() == "name")
        {            
            QStandardItem* item = SetChild(tree_item, element.text());
            int row_num=list_model->rowCount();
            list_model->insertRow(row_num);
            list_model->setData(list_model->index(row_num), element.text());

            tree_to_list.insert(item->index(), list_model->index(row_num, 0));
            ListItemData list_item_data;
            list_item_data.tree_item = item->index();

            QDomNode properties_node_partent = dom_node.parentNode();
            QDomNode prop = properties_node_partent.toElement().firstChild();

            while(!prop.isNull())
            {
                StringPair pair;
                pair.name = prop.toElement().tagName();

                if(pair.name.toLower() == "size")
                {
                    QDomNode size_child = prop.toElement().firstChild();

                    while(!size_child.isNull())
                    {
                        QString y = size_child.toElement().text();
                        pair.value += pair.value.length() == 0 ? size_child.toElement().text() : "x" + size_child.toElement().text();
                        size_child = size_child.nextSibling();
                    }
                }
                else
                {
                    pair.value = prop.toElement().text();
                }
                list_item_data.list_item_data.append(pair);
                prop = prop.nextSibling();
            }

            list_to_tree.insert(list_model->index(row_num, 0), list_item_data);
        }
    }
}

QStandardItem* ModelBuilder::SetChild(QStandardItem* parent, QString name)
{
    if(parent == nullptr)
    {
        parent = new QStandardItem(name);

        if(tree_model->rowCount() == 0)
        {
            tree_model->setItem(0, 0, parent);
            tree_model->setHeaderData(0, Qt::Horizontal, "Assembly Structure");
        }
        else
        {
            tree_model->setItem(tree_model->rowCount(), 0, parent);
        }

        return parent;
    }

    QStandardItem* child = new QStandardItem(name);
    parent->setChild(parent->rowCount(), 0, child);
    return child;
}


