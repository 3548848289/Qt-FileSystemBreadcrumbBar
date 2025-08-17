// QBreadcrumbBar.h
#pragma once
#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMenu>

struct BreadcrumbNode {
    QString name;
    QList<BreadcrumbNode*> children;
    BreadcrumbNode(const QString& n) : name(n) {}
};

class QBreadcrumbBar : public QWidget {
    Q_OBJECT
public:
    explicit QBreadcrumbBar(QWidget* parent = nullptr);
    void setPath(const QList<BreadcrumbNode*>& path);
    void addMenuItem(int index, const QString& text);

signals:
    void pathClicked(int index, const QString& name);

private:
    QHBoxLayout* layout;
    QList<BreadcrumbNode*> currentPath;

    void clearLayout();
    void rebuild();
};
