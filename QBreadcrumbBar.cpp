#include "QBreadcrumbBar.h"

QBreadcrumbBar::QBreadcrumbBar(QWidget* parent) : QWidget(parent) {
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    setLayout(layout);
}

void QBreadcrumbBar::setShowFiles(bool value) {
    showFiles = value;
    rebuild();
}

void QBreadcrumbBar::setPath(const QList<BreadcrumbNode*>& path) {
    currentPath = path;
    rebuild();
}

void QBreadcrumbBar::clearLayout() {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
}

void QBreadcrumbBar::rebuild() {
    clearLayout();

    for (int i = 0; i < currentPath.size(); ++i) {
        BreadcrumbNode* node = currentPath[i];

        // 面包屑按钮
        QToolButton* btn = new QToolButton(this);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        // btn->setAutoRaise(true);
        btn->setStyleSheet("border: none; background: transparent;");

        btn->setText(node->name);

        connect(btn, &QToolButton::clicked, this, [this, i](){
            currentPath = currentPath.mid(0, i+1);
            rebuild();
            emit pathClicked(i, currentPath[i]->name);
        });
        layout->addWidget(btn);

        // 填充子节点
        populateChildren(node);

        if (!node->children.isEmpty()) {
            QToolButton* sepBtn = new QToolButton(this);
            sepBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            // sepBtn->setAutoRaise(true);
            sepBtn->setStyleSheet("border: none; background: transparent;");

            sepBtn->setText("/");
            layout->addWidget(sepBtn);

            QMenu* menu = new QMenu(this);
            for (BreadcrumbNode* child : node->children) {
                QAction* action = menu->addAction(child->name);
                connect(action, &QAction::triggered, this, [this, i, child](){
                    if (!child->fullPath.isEmpty() && QFileInfo(child->fullPath).isFile()) {
                        emit fileClicked(child->fullPath);
                    } else {
                        QList<BreadcrumbNode*> newPath = currentPath.mid(0, i+1);
                        newPath.append(new BreadcrumbNode(child->name, child->fullPath, child->isVirtualRoot));
                        setPath(newPath);
                        emit pathClicked(i+1, child->name);
                    }
                });
            }

            connect(sepBtn, &QToolButton::clicked, this, [sepBtn, menu](bool){
                menu->exec(sepBtn->mapToGlobal(QPoint(0, sepBtn->height())));
            });
        }
    }

    layout->addStretch();
}

void QBreadcrumbBar::addMenuItem(int index, const QString& text) {
    if (index < 0 || index >= currentPath.size()-1) return;
    BreadcrumbNode* node = currentPath[index];
    BreadcrumbNode* newChild = new BreadcrumbNode(text);
    node->children.append(newChild);
    rebuild();
}

void QBreadcrumbBar::populateChildren(BreadcrumbNode* node) {
    BreadcrumbNodeHelper::populateChildren(node, showFiles);
}
