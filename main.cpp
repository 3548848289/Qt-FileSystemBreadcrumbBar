#include <QApplication>
#include "QBreadcrumbBar.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    QHBoxLayout* mainLayout = new QHBoxLayout(&window);

    QBreadcrumbBar* breadcrumb = new QBreadcrumbBar();
    mainLayout->addWidget(breadcrumb);

    // 构建树
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

    QList<BreadcrumbNode*> path = {root, phone, smart};
    breadcrumb->setPath(path);


    window.show();
    return app.exec();
}
