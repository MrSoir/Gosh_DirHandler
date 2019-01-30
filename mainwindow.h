#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>

#include <vector>
#include <string>

#include "file_handling_src/dirmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
public slots:
    void receiveStrings(std::vector<std::string> ss);
    void treeChanged(FiBD);
    void dirChanged(FiBD changed_dir);

    void elapseRoot();
    void elapse();
    void elapseRec();
    void collapse();
    void collapseRec();
    void deleteDir();

    void search();
    void receiveDeepSearchResults(std::vector<std::string> matchingPaths, std::string keyword);
private:
    void createLayout();

    void iterateTree(FiBD dir, int id=0);

    void addTextToView(const std::string& s, bool isSearched = false);

    void addButtons();

    void connectManager();

    std::string getSelectedPath() const;

    bool replaceTreeNode(FiBD& dir_src, FiBD& dir_to_replace);

    void revalidateListView();

    Ui::MainWindow *ui;
    std::string root_path = "/home/hippo/Documents/tests_src";
    QListWidget* listWidget;
    QLineEdit* searchLineEdit;
    QCheckBox* searchCheckBox;
    QVBoxLayout* mainLayout;

    std::unordered_set<std::string> searchResults;
    QColor searchedColor = QColor(255, 0, 0);

    DirManager* manager;
    std::vector<QPushButton*> btns;

    FiBD m_tree;
};

#endif // MAINWINDOW_H
