#include "set_measurement_unit.h"
#include "ui_set_measurement_unit.h"
#include <QStringList>
#include <QComboBox>
#include <QString>
#include <QLabel>
#include <QFontDatabase>
#include <QFont>
#include <QHeaderView>
#include <QAbstractItemView>


set_measurement_unit::set_measurement_unit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::set_measurement_unit)
{
    ui->setupUi(this);

    /*************************设置样式表************************************/
    setStyleSheet("QTableWidget{border-width:1px;border-color:rgb(48, 104, 151);\
                  border-style: outset;background-color: rgb(132, 171, 208);\
                  background: transparent;background-image: url(:/Image/Ressources/MainWindow.png);}\
                  QComboBox,QLabel{font-family:\"Times New Roman\";font-size:25px;\
                  } ");


    change_unit=ui->unit_widget;
    change_unit->setFont(QFont("Times New Roman",30));

     change_unit->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    // change_unit->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    // change_unit->verticalHeader()->setDefaultSectionSize(10); //设置行高
     //change_unit->setFrameShape(QFrame::NoFrame); //设置无边框
     //change_unit->setShowGrid(false); //设置不显示格子线
    // change_unit->verticalHeader()->setVisible(false); //设置垂直头不可见
     change_unit->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
     change_unit->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
     change_unit->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
     //change_unit->horizontalHeader()->resizeSection(0,150); //设置表头第一列的宽度为150
     //change_unit->horizontalHeader()->setFixedHeight(25); //设置表头的高度

     change_unit->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

     change_unit->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
     change_unit->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色

    //设置初始的行和列数
    change_unit->setRowCount(8);
    change_unit->setColumnCount(3);
    QStringList row;
    QStringList col;
    row<<"位移"<<"速度"<<"加速度"<<"力"<<"质量"<<"量级"<<"电压"<<"自定义";
    col<<"显示单位"<<"标准单位"<<"转换系数";

    change_unit->setHorizontalHeaderLabels(col);
    change_unit->setVerticalHeaderLabels(row);

    QStringList Item;
    /****************设置表格的初始数据********************************************/

    change_unit->setCellWidget(0,1,new QLabel("m"));
    change_unit->setCellWidget(1,1,new QLabel("m/s"));
    change_unit->setCellWidget(2,1,new QLabel("m/s2"));
    change_unit->setCellWidget(3,1,new QLabel("N"));
    change_unit->setCellWidget(4,1,new QLabel("kg"));
    change_unit->setCellWidget(5,1,new QLabel("ratio"));
    change_unit->setCellWidget(6,1,new QLabel("V"));

    change_unit->setCellWidget(0,2,new QLabel("0.001"));
    change_unit->setCellWidget(1,2,new QLabel("1"));
    change_unit->setCellWidget(2,2,new QLabel("9.8"));
    change_unit->setCellWidget(3,2,new QLabel("1"));
    change_unit->setCellWidget(4,2,new QLabel("1"));
    change_unit->setCellWidget(5,2,new QLabel(""));
    change_unit->setCellWidget(6,1,new QLabel("1"));
    /*************下面为整个表格添加选择控件提供表格的选择功能*************************/
    //位移单位
    QComboBox *pos = new QComboBox;
    //速度单位
    QComboBox *vec = new QComboBox;
    //加速度单位
    QComboBox *acc = new QComboBox;
    //力
    QComboBox *f = new QComboBox;
    //质量
    QComboBox *weight = new QComboBox;
    //量级
    QComboBox *rate = new QComboBox;
    //电压
    QComboBox *vol = new QComboBox;
    //自定义
    QComboBox *EU = new QComboBox;
    Item<<"m"<<"cm"<<"mm"<<"Ft"<<"In";
    pos->addItems(Item);
    Item.clear();
    Item<<"m/s"<<"cm/s"<<"mm/s"<<"Ft/s"<<"In/s";
    vec->addItems(Item);
    Item.clear();
    Item<<"g"<<"m/s2"<<"cm/s2"<<"mm/s2"<<"Ft/s2"<<"In/s2";
    acc->addItems(Item);
    Item.clear();
    Item<<"N"<<"LBF";
    f->addItems(Item);
    Item.clear();
    Item<<"Kg"<<"lbs"<<"Gram"<<"Ounce"<<"Ton";
    weight->addItems(Item);
    Item.clear();
    Item<<"%"<<"‰";
    rate->addItems(Item);
    Item.clear();
    Item<<"V"<<"mV"<<"uV";
    vol->addItems(Item);
    Item<<"EU";
    EU->addItems(Item);

    /*****************************下面往表格中添加控件***************************/
    change_unit->setCellWidget(0,0,pos);
    change_unit->setCellWidget(1,0,vec);
    change_unit->setCellWidget(2,0,acc);
    change_unit->setCellWidget(3,0,f);
    change_unit->setCellWidget(4,0,weight);
    change_unit->setCellWidget(5,0,rate);
    change_unit->setCellWidget(6,0,vol);
    change_unit->setCellWidget(7,0,EU);



}

set_measurement_unit::~set_measurement_unit()
{
    delete ui;
}
