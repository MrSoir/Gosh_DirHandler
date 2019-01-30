#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    manager(new DirManager(root_path))
{
    ui->setupUi(this);
    createLayout();

    setMinimumSize(QSize(600,600));

    connectManager();
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";
    delete ui;
    manager->close();
}

void MainWindow::receiveStrings(std::vector<std::string> ss)
{
    listWidget->clear();
    for(const auto& s: ss)
    {
        new QListWidgetItem(QString::fromStdString(s), listWidget);
    }
}

void MainWindow::treeChanged(FiBD dir)
{
    qDebug() << "MainWindow::treeChanged";
    m_tree = dir;
    revalidateListView();
}

void MainWindow::dirChanged(FiBD changed_dir)
{
    qDebug() << "MainWindow::dirChanged";
    replaceTreeNode(m_tree, changed_dir);
    revalidateListView();
}

void MainWindow::elapseRoot()
{
    manager->elapseRec(root_path);
}

void MainWindow::elapse()
{
    std::string selection = getSelectedPath();
    if( !selection.empty() )
    {
        manager->elapse(selection);
    }
}

void MainWindow::elapseRec()
{
    std::string selection = getSelectedPath();
    if( !selection.empty() )
    {
        manager->elapseRec(selection);
    }
}

void MainWindow::collapse()
{
    std::string selection = getSelectedPath();
    if( !selection.empty() )
    {
        manager->collapse(selection);
    }
}

void MainWindow::collapseRec()
{
    std::string selection = getSelectedPath();
    if( !selection.empty() )
    {
        manager->collapseRec(selection);
    }
}

void MainWindow::deleteDir()
{
    std::string selection = getSelectedPath();
    if( !selection.empty() )
    {
        manager->deleteDir(selection);
    }
}

void MainWindow::search()
{
    std::string search_key_word = searchLineEdit->text().toStdString();
    bool includeHiddenFiles = searchCheckBox->isChecked();
    manager->deepSearch(search_key_word, includeHiddenFiles);
}

void MainWindow::receiveDeepSearchResults(std::vector<string> matchingPaths, string keyword)
{
    qDebug() << "MainWindow::receiveDeepSearchResults - keyword: " << QString::fromStdString(keyword)
             << "   searchResults: " << matchingPaths.size();

    searchResults.clear();
    for(const auto& match: matchingPaths)
    {
        searchResults.insert(match);
    }

    revalidateListView();
}


void MainWindow::createLayout()
{
    mainLayout = new QVBoxLayout();

    listWidget = new QListWidget();
    new QListWidgetItem(QString::fromStdString(root_path), listWidget);
    mainLayout->addWidget(listWidget);

    QWidget* helper = new QWidget(this);
    helper->setLayout(mainLayout);
    this->setCentralWidget(helper);

    addButtons();

    searchLineEdit = new QLineEdit();
    searchCheckBox = new QCheckBox("include hidden files");
    QPushButton* searchBtn = new QPushButton(tr("search!"));
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::search);

    QVBoxLayout* searchLayout = new QVBoxLayout();
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchCheckBox);
    searchLayout->addWidget(searchBtn);
    QGroupBox* searchGrpBox = new QGroupBox(tr("deep search"));
    searchGrpBox->setLayout(searchLayout);
    mainLayout->addWidget(searchGrpBox);
}

void MainWindow::iterateTree(FiBD dir, int id)
{
    std::string prepend = "";
    for(int i=0; i < id; ++i)
    {
        prepend.append("    ");
    }

    std::string dir_txt = prepend + dir.abs_path;
    bool isSearched = (searchResults.find(dir.abs_path) != searchResults.end());
    addTextToView(dir_txt, isSearched);

    if( dir.abs_path != root_path && !dir.isElapsed )
        return;

    for(const auto& sub_dir: dir.folders)
    {
        iterateTree(sub_dir, id+1);
    }
    for(const auto& file: dir.files)
    {
        std::string txt = prepend + "   file:" + file;
        isSearched = (searchResults.find(file) != searchResults.end());
        addTextToView(txt, isSearched);
    }
}

void MainWindow::addTextToView(const std::string& s, bool isSearched)
{
    QListWidgetItem* listItm = new QListWidgetItem(QString::fromStdString(s), listWidget);
    if(isSearched)
        listItm->setTextColor(searchedColor);

}

void MainWindow::addButtons()
{
    QPushButton* elapseBtn = new QPushButton("elapse");
    connect(elapseBtn, &QPushButton::clicked, this, &MainWindow::elapse);
    mainLayout->addWidget(elapseBtn);

    QPushButton* elapseRecBtn = new QPushButton("elapse recursive");
    connect(elapseRecBtn, &QPushButton::clicked, this, &MainWindow::elapseRec);
    mainLayout->addWidget(elapseRecBtn);

    QPushButton* collapseBtn = new QPushButton("collapse");
    connect(collapseBtn, &QPushButton::clicked, this, &MainWindow::collapse);
    mainLayout->addWidget(collapseBtn);

    QPushButton* collapseRecBtn = new QPushButton("collapse recursive");
    connect(collapseRecBtn, &QPushButton::clicked, this, &MainWindow::collapseRec);
    mainLayout->addWidget(collapseRecBtn);

    QPushButton* deleteBtn = new QPushButton("delete");
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteDir);
    mainLayout->addWidget(deleteBtn);
}

void MainWindow::connectManager()
{
    connect(manager, &DirManager::treeChanged, this, &MainWindow::treeChanged, Qt::QueuedConnection);
    connect(manager, &DirManager::dirChanged, this, &MainWindow::dirChanged, Qt::QueuedConnection);
    connect(manager, &DirManager::deepSearchFinished, this, &MainWindow::receiveDeepSearchResults, Qt::QueuedConnection);
}

std::string MainWindow::getSelectedPath() const
{
    auto selectedItms = listWidget->selectedItems();
    if(selectedItms.size() > 0)
    {
        auto itm = selectedItms[0];
        return StringOps::trim(itm->text().toStdString());
    }
    return "";
}

bool MainWindow::replaceTreeNode(FiBD& dir_src, FiBD& dir_to_replace)
{
    if(dir_src.abs_path == dir_to_replace.abs_path)
    {
        dir_src.replaceContents(dir_to_replace);
        return true;
    }
    for(auto& sub_dir: dir_src.folders)
    {
        if(replaceTreeNode(sub_dir, dir_to_replace))
        {
            return true;
        }
    }
    return false;
}

void MainWindow::revalidateListView()
{
    listWidget->clear();
    iterateTree(m_tree);
}
