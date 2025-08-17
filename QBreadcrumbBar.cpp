
// QBreadcrumbBar.cpp
#include "QBreadcrumbBar.h"

QBreadcrumbBar::QBreadcrumbBar(QWidget* parent) : QWidget(parent) {
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(2);
    setLayout(layout);
}

void QBreadcrumbBar::clearLayout() {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void QBreadcrumbBar::setPath(const QList<BreadcrumbNode*>& path) {
    currentPath = path;
    rebuild();
}

void QBreadcrumbBar::rebuild() {
    clearLayout();
    for (int i = 0; i < currentPath.size(); ++i) {
        BreadcrumbNode* node = currentPath[i];

        // 面包屑按钮
        QToolButton* btn = new QToolButton(this);
        btn->setText(node->name);
        btn->setProperty("index", i);
        connect(btn, &QToolButton::clicked, this, [this, i]() {
            currentPath = currentPath.mid(0, i+1);
            rebuild();
            emit pathClicked(i, currentPath[i]->name);
        });
        layout->addWidget(btn);

        // 只要节点有子节点，或者是最后一个节点也保留 >
        if (!node->children.isEmpty() || i == currentPath.size() - 1) {
            QToolButton* sepBtn = new QToolButton(this);
            sepBtn->setText(">");
            sepBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
            layout->addWidget(sepBtn);

            QMenu* menu = new QMenu(sepBtn);
            for (BreadcrumbNode* child : node->children) {
                QAction* action = menu->addAction(child->name);
                connect(action, &QAction::triggered, [this, i, child]() {
                    QList<BreadcrumbNode*> newPath = currentPath.mid(0, i+1);
                    newPath.append(child);
                    setPath(newPath);
                    emit pathClicked(i+1, child->name);
                });
            }
            connect(sepBtn, &QToolButton::clicked, [sepBtn, menu]() {
                menu->exec(sepBtn->mapToGlobal(QPoint(0, sepBtn->height())));
            });
        }
    }
    layout->addStretch();
}

// 动态添加菜单项到某个分隔符
void QBreadcrumbBar::addMenuItem(int index, const QString& text) {
    if (index < 0 || index >= currentPath.size()-1) return;
    BreadcrumbNode* node = currentPath[index];
    BreadcrumbNode* newChild = new BreadcrumbNode(text);
    node->children.append(newChild);
    rebuild();
}
