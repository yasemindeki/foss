#include "qinboxmessageheader.h"
#include "subwidgets/qeventbutton.h"
#include "subwidgets/qcategorybutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QToolBar>

QInboxMessageHeader::QInboxMessageHeader(QWidget *parent) : QFrame(parent)
{
    //this->setFrameStyle(QFrame::Panel | QFrame::Raised);
    this->setObjectName("QInboxMessageHeader");
    this->setStyleSheet("QWidget#QInboxMessageHeader { \
                            background-color: rgb(250, 250, 250); \
                            border-bottom: 1px solid rgb(229, 229, 229); \
                            border-left: 1px solid rgb(229, 229, 229); \
                            padding-left: 50px; \
                            padding-right: 5px; \
                            padding-top: 10px; \
                            padding-bottom: 10px; \
                        }");

    QVBoxLayout *headerLayout = new QVBoxLayout();
    headerLayout->setSpacing(10);

    subjectLabel = new QLabel(tr("Mesajın konusu burada yazilsin"));
    QFont subjectFont("Segoe UI", 10, QFont::Bold);
    subjectFont.setStyleStrategy(QFont::PreferAntialias);
    subjectLabel->setFont(subjectFont);

    numBodies = new QLabel("2");
    QFont numFont("Lucida Sans Typewriter", 11, QFont::Bold);
    numBodies->setFont(numFont);
    numBodies->setFixedSize(20, 20);
    numBodies->setStyleSheet("color: white; background-color: rgb(36, 41, 46); border: 1px solid rgb(36, 41, 46); border-radius: 10px;");
    numBodies->setAlignment(Qt::AlignHCenter);

    // [TODO] tek mesaj icermiyor ise gostermeli
    QHBoxLayout *subjectLayout = new QHBoxLayout();
    subjectLayout->setSpacing(5);
    subjectLayout->addWidget(subjectLabel);
    subjectLayout->addWidget(numBodies);
    subjectLayout->addStretch();

    createCategories();

    headerLayout->addLayout(subjectLayout);
    headerLayout->addWidget(tbCategories);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QEventButton *printButton = new QEventButton(tr("Print this email"), this);
    printButton->setIcons(":/icons/print-dark.png", ":/icons/print-white.png");

    QEventButton *zoomButton = new QEventButton(tr("Zoom options"), this);
    zoomButton->setIcons(":/icons/zoom-dark.png", ":/icons/zoom-white.png");

    QEventButton *moreButton = new QEventButton(tr("More options"), this);
    moreButton->setIcons(":/icons/more-dark.png", ":/icons/more-white.png");

    mainLayout->addLayout(headerLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(zoomButton);
    mainLayout->addWidget(printButton);
    mainLayout->addWidget(moreButton);

    this->setLayout(mainLayout);
    this->setMinimumHeight(63);
}

void QInboxMessageHeader::createCategories()
{
    tbCategories = new QToolBar();
    tbCategories->setMovable(false);
    tbCategories->setStyleSheet("QToolBar { \
                                    spacing: 2px; \
                                    background-color: rgb(250, 250, 250); \
                                    border: none; \
                                   }");

    //QAction *actionCategoryLonca = new QAction(QIcon(":/icons/tobold-14px.png"), tr("Format text to &bold"), this);
    //actionCategoryLonca->setCheckable(false);

    QCategoryButton *tbCategoryLonca = new QCategoryButton("Lonca", Qt::green);
    QCategoryButton *tbCategoryMarketing = new QCategoryButton("Marketing", Qt::blue);
    QCategoryButton *tbCategoryCustomers = new QCategoryButton("Customers", Qt::red);

    tbCategories->addWidget(tbCategoryLonca);
    tbCategories->addWidget(tbCategoryMarketing);
    tbCategories->addWidget(tbCategoryCustomers);

    //tbCategoryLonca->setDefaultAction(actionCategoryLonca);
    //this->connect(tbCategoryLonca, SIGNAL(clicked()), this, SLOT(onCategoryLoncaClicked()));
    //this->connect(actionCategoryLonca, SIGNAL(triggered()), this, SLOT(onCategoryLoncaClicked()));
}
