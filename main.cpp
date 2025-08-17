#include <QApplication>
#include <QStyleHints>
#include <QScrollArea>
#include "QBreadcrumbBar.h"

QWidget* createFileSystemDemo() {
    QWidget* window = new QWidget;
    window->setFixedHeight(50);
    window->setStyleSheet("QWidget { border: 1px solid lightgray; border-radius: 2px; }");

    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);

    QBreadcrumbBar* breadcrumb = new QBreadcrumbBar();
    breadcrumb->setShowFiles(true);

    // 文件系统导航：虚拟根（显示盘符）
    BreadcrumbNode* root = new BreadcrumbNode("计算机", "", true);
    QList<BreadcrumbNode*> path = { root };
    breadcrumb->setPath(path);

    // 用 QScrollArea 包裹 breadcrumb
    QScrollArea* scrollArea = new QScrollArea(window);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(breadcrumb);

    layout->addWidget(scrollArea); // 把 scrollArea 加到布局，而不是 breadcrumb 本身

    return window;
}


QWidget* createCustomTreeDemo() {
    QWidget* window = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(window);

    QBreadcrumbBar* breadcrumb = new QBreadcrumbBar();
    layout->addWidget(breadcrumb);

    // 自定义导航树
    BreadcrumbNode* root = new BreadcrumbNode("电子产品");
    BreadcrumbNode* phone = new BreadcrumbNode("手机");
    BreadcrumbNode* computer = new BreadcrumbNode("电脑");
    BreadcrumbNode* hear = new BreadcrumbNode("耳机");
    BreadcrumbNode* smart = new BreadcrumbNode("智能手机");
    BreadcrumbNode* flag1 = new BreadcrumbNode("旗舰系列");
    BreadcrumbNode* flag2 = new BreadcrumbNode("Note系列");

    root->children.append(phone);
    root->children.append(computer);
    root->children.append(hear);
    phone->children.append(smart);
    smart->children.append(flag1);
    smart->children.append(flag2);

    QList<BreadcrumbNode*> path = { root, phone, smart };
    breadcrumb->setPath(path);

    return window;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qApp->styleHints()->setColorScheme(Qt::ColorScheme::Light);

    QWidget window;
    QVBoxLayout* mainLayout = new QVBoxLayout(&window);

    // 文件系统面包屑
    QWidget* fileSystemDemo = createFileSystemDemo();
    mainLayout->addWidget(fileSystemDemo);

    // 自定义逻辑树面包屑
    QWidget* customTreeDemo = createCustomTreeDemo();
    mainLayout->addWidget(customTreeDemo);

    window.resize(600, 200);
    window.show();

    return app.exec();
}
