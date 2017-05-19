#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    modelFile = new QStandardItemModel();
    modelFileSecond = new QStandardItemModel();
    ui->tableView->setModel(modelFile);
     ui->tableView_2->setModel(modelFileSecond);
    size = -1;
    sizeSecond = -1;
    setTableHeader();
    this->ui->pushButton_19->setText("中文界面");
    UIisCN=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setTableHeader()
{
    QStringList list ;
    list.append("0");
    list.append("1");
    list.append("2");
    list.append("3");
    list.append("4");
    list.append("5");
    list.append("6");
    list.append("7");
    list.append("8");
    list.append("9");
    list.append("A");
    list.append("B");
    list.append("C");
    list.append("D");
    list.append("E");
    list.append("F");

    modelFile->setHorizontalHeaderLabels(list);
    modelFileSecond->setHorizontalHeaderLabels(list);
//    ui->tableView->setModel(modelFile);
//     ui->tableView_2->setModel(modelFileSecond);
    for(int i=0;i<16;i++)
    {  // if(i==7)
        //{
       //     ui->tableView->setColumnWidth(i,80);
       //     ui->tableView_2->setColumnWidth(i,80);
       // }else
        {
            ui->tableView->setColumnWidth(i,50);
            ui->tableView_2->setColumnWidth(i,50);
        }
    }
}

void MainWindow::openFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,"open file dialog","","");

    if(!fileName.isEmpty())
    {
        fileFirstSaveStr = fileName;
        modelFile->clear();// can  decide memory use when open many times
        setTableHeader();
        fileObject.setFileName(fileName);
        ui->label->setText(fileName);
        size=0;
        fileObject.open(QIODevice::ReadOnly);
        size=fileObject.size();
        fileObject.close();
        fileList.clear();
        int m=0;
        int n=0;
         char *readByte=new char[1];//读取1字节
         fileObject.open(QIODevice::ReadOnly);
         QDataStream  dataStreamObject(&fileObject);
         QString str;
         for(int i=0;i<size;i++)
         {
             fileObject.seek(i);
             dataStreamObject.readRawData(readByte,1);
             quint8 number=(quint8)readByte[0];
             n=i/16;
             m=i-n*16;

             str=QString("%1").arg(number, 2, 16, QLatin1Char('0')) ;
             modelFile->setItem(n,m,new QStandardItem(str.toUpper()));
             modelFile->item(n,m)->setBackground(QBrush(QColor(128,128,128)));

         //    fileList.append(number);

         }
        fileObject.close();
         row = n;
         columnEndAt =m;
         delete readByte;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    openFile();
}

void MainWindow::on_pushButton_5_clicked()
{  //search
    if(size > 0)
    {
        QString str=ui->lineEdit->text();
        str=str.toUpper();
        ui->lineEdit->setText(str);

        needFindStr = str;
        int sizeFind = needFindStr.length();
        int strWidth = sizeFind/2;

        findResault.clear();//查找队列清空

        QAbstractItemModel *model = ui->tableView->model();
        QModelIndex index ;
        QVariant dataCell ;
        for(int i=0;i<=row;i++)
        {
            for(int j=0;j<16;j++)
            {
                int pos=i*16+j+strWidth-1;
                if((pos<size) && (strWidth>0))//保证数据不越界，且字符串长度大于1字节
                {
                    if((j+strWidth-1)<16)//不跨行时
                    {
                        QString strTemp="";
                        QString strOneByte="";
                        for(int k=j;k<(strWidth+j);k++)
                        {
                            index = model->index(i,k);
                            dataCell= model->data(index);
                            strOneByte = dataCell.toString();
                            strTemp=strTemp+strOneByte.toUpper();
                        }
                     //  qDebug()<<strTemp<<"1  line";

                        if((strTemp.compare(needFindStr))==0)
                        {
                                recPosition singleRec;
                                singleRec.row = i;
                                singleRec.columnStart = j;
                                findResault.append(singleRec);
                        }else
                        {
                            continue ;
                        }

                    }else //跨行时
                    {
                                int nextLineNumber=(j+strWidth-1)-15;//折行后应取的单元格数量
                                int thisLineNumber=strWidth-nextLineNumber;//本行应取的单元格数量
                                QString strTemp="";
                                QString strOneByte="";
                                for(int k=j;k<=(thisLineNumber+j);k++)
                                {
                                    index = model->index(i,k);
                                    dataCell= model->data(index);
                                    strOneByte = dataCell.toString();
                                    strTemp=strTemp+strOneByte.toUpper();
                                }

                                for(int k=0;k<nextLineNumber;k++)
                                {
                                    index = model->index(i+1,k);
                                    dataCell= model->data(index);
                                    strOneByte = dataCell.toString();
                                    strTemp=strTemp+strOneByte.toUpper();
                                }

                            //    qDebug()<<strTemp<<"2 line";

                                 if((strTemp.compare(needFindStr))==0)
                                 {
                                         recPosition singleRec;
                                         singleRec.row = i;
                                         singleRec.columnStart = j;
                                         findResault.append(singleRec);
                                 }else
                                 {
                                     continue ;
                                 }


                    }

                }else
                {
                    break ;
                }
                ;
            }
        }

        qDebug()<<row<<"   "<<columnEndAt<<"  file size"<<size;

        int resaultCount=findResault.count();
        if(resaultCount>0)
        {
            int  rowTemp=findResault[0].row;
            int colTemp=findResault[0].columnStart;
            index = model->index(rowTemp,colTemp);
            ui->tableView->setCurrentIndex(index);
            displaySearchPos=0;

            QString str3;
            str3 = QTime::currentTime().toString();
            str3= "find it   "+str3;
            ui->label_3->setText(str3);



        }else
        {
            QString str3;
            str3 = QTime::currentTime().toString();
            str3= "can not find it   "+str3;
            ui->label_3->setText(str3);
        }


    /*
        QAbstractItemModel *model = ui->tableView->model();
        QModelIndex index = model->index(0,0);
        QVariant data = model->data(index);
         str=data.toString();
         ui->lineEdit->setText(str);
          bool ok;
          int hex = str.toInt(&ok, 16);       // hex == 255, ok == true
         qDebug()<<hex;
    */
   }
}

void MainWindow::on_pushButton_6_clicked()
{    //next
    if(findResault.count()>0)
    {

         displaySearchPos=displaySearchPos+1;
         int max = findResault.count()-1;
         int min = 0;
         if(displaySearchPos > max)
         {
              displaySearchPos = max;
         }

         if(displaySearchPos < min)
         {
             displaySearchPos =min;
         }

         QAbstractItemModel *model = ui->tableView->model();
         QModelIndex index ;

         int  rowTemp=findResault[displaySearchPos].row;
         int colTemp=findResault[displaySearchPos].columnStart;
         index = model->index(rowTemp,colTemp);
         ui->tableView->setCurrentIndex(index);
    }
       printSearchResault();
//    QString  str1="6676";
//    QString str2="6677";
//    qDebug()<<str1.compare(str2);
}

void  MainWindow::printSearchResault()
{
    int resaultCount=findResault.count();
    if(resaultCount>0)
    {
        for(int i=0;i<resaultCount;i++)
        {
            qDebug()<<" find position: "<<findResault[i].row<< "  "<<findResault[i].columnStart;
        }
    }

}

void MainWindow::on_pushButton_7_clicked()
{
     //before
    if(findResault.count()>0)
    {

         displaySearchPos=displaySearchPos-1;
         int max = findResault.count()-1;
         int min = 0;
         if(displaySearchPos > max)
         {
              displaySearchPos = max;
         }

         if(displaySearchPos < min)
         {
             displaySearchPos =min;
         }

         QAbstractItemModel *model = ui->tableView->model();
         QModelIndex index ;

         int  rowTemp=findResault[displaySearchPos].row;
         int colTemp=findResault[displaySearchPos].columnStart;
         index = model->index(rowTemp,colTemp);
         ui->tableView->setCurrentIndex(index);
    }
}

void MainWindow::saveFile()
{
      if(size >0)
      {
          QString fileName;
          if(!fileFirstSaveStr.isEmpty())
          {
              //fileName = "/1_1.dat";
              fileName = fileFirstSaveStr;
          }

          fileObject.setFileName(fileName);
          char *writeByte=new char[1];//write 1 byte
          fileObject.open(QIODevice::WriteOnly);

          QDataStream  dataStreamObject(&fileObject);

          for(int i=0;i<=row;i++)
          {
              for(int j=0;j<16;j++)
              {
                  QAbstractItemModel *model = ui->tableView->model();
                  QModelIndex index ;
                  QString str;
                  quint8 hex;
                   bool ok;
                   if( (i==row) &&  (j==columnEndAt) )
                   {
                       index = model->index(i,j);
                       QVariant data = model->data(index);
                       str=data.toString();
                       hex = str.toInt(&ok, 16);       // hex == 255, ok == true
                       writeByte[0]=hex;
                       fileObject.seek(i*16+j);
                       dataStreamObject.writeRawData(writeByte,1);
                       break ;
                   }else
                   {
                        index = model->index(i,j);
                        QVariant data = model->data(index);
                        str=data.toString();
                        hex = str.toInt(&ok, 16);       // hex == 255, ok == true
                        if(!ok)
                        {
                           QMessageBox::warning(NULL,"Input Error  !", "not a hex char!");
                            break ;
                        }
                        writeByte[0]=hex;
                        fileObject.seek(i*16+j);
                        dataStreamObject.writeRawData(writeByte,1);
                   }

              }
          }

    //      for(int i=0;i<size;i++)
    //      {

    //          fileObject.seek(i);
    //          dataStreamObject.writeRawData(writeByte,1);

    //       //   quint8 number=(quint8)readByte[0];
    //      }


          fileObject.close();
      //  fileObject.rename("1_1.dat","2_2.dat");

          delete writeByte;
      }
}

void MainWindow::on_pushButton_8_clicked()
{

       saveFile();
}

void MainWindow::on_pushButton_9_clicked()
{    //save search
      int size_Temp = findResault.count();
      findResaultSave.clear();
      if(size_Temp>0)
      {

          for(int i=0;i<size_Temp;i++)
          {
              findResaultSave.append(findResault[i]);
          }
          qDebug()<<" findResault save sucess !";
          QString str3;
          str3 = QTime::currentTime().toString();
          str3= "findResault save sucess    "+str3;
          ui->label_3->setText(str3);
      }else
      {

          QString str3;
          str3 = QTime::currentTime().toString();
          str3= "no  findResault  need  save  "+str3;
          ui->label_3->setText(str3);
          return ;
      }
}

void MainWindow::on_pushButton_10_clicked()
{// search in SaveSearch
    QString strSearch;
    strSearch = ui->lineEdit_2->text().toUpper();
    int sizeFind = strSearch.length();
    int strWidth = sizeFind/2;
    QList <recPosition> tempList;
    tempList.clear();
    QAbstractItemModel *model = ui->tableView->model();
    QModelIndex index ;
    QVariant dataCell ;
    if((size >0) && (findResaultSave.count()>0))//File has open ,and search resault has save
    {
         int saveMax= findResaultSave.count();
         for(int i=0;i<saveMax;i++)
         {
                int currentRow=findResaultSave[i].row;
                int currentColumn=findResaultSave[i].columnStart;
                int pos = currentRow*16+currentColumn+strWidth-1;
                if((pos<size)&&(strWidth>0))
                {
                        if((currentColumn+strWidth-1)<16)// in one line
                        {
                            QString strTemp="";
                            QString strOneByte="";
                            for(int k=currentColumn;k<(strWidth+currentColumn);k++)
                            {
                                index = model->index(currentRow,k);
                                dataCell= model->data(index);
                                strOneByte = dataCell.toString();
                                strTemp=strTemp+strOneByte.toUpper();
                            }

                            if((strTemp.compare(strSearch))==0)
                            {
                                    recPosition singleRec;
                                    singleRec.row = currentRow;
                                    singleRec.columnStart = currentColumn;
                                    tempList.append(singleRec);
                            }else
                            {
                                continue ;
                            }
                        }else  //in  two line
                        {
                            int nextLineNumber=(currentColumn+strWidth-1)-15;//折行后应取的单元格数量
                            int thisLineNumber=strWidth-nextLineNumber;//本行应取的单元格数量
                            QString strTemp="";
                            QString strOneByte="";
                            for(int k=currentColumn;k<=(thisLineNumber+currentColumn);k++)
                            {
                                index = model->index(currentRow,k);
                                dataCell= model->data(index);
                                strOneByte = dataCell.toString();
                                strTemp=strTemp+strOneByte.toUpper();
                            }
                            for(int k=0;k<nextLineNumber;k++)
                            {
                                index = model->index(currentRow+1,k);
                                dataCell= model->data(index);
                                strOneByte = dataCell.toString();
                                strTemp=strTemp+strOneByte.toUpper();
                            }


                            if((strTemp.compare(strSearch))==0)
                            {
                                    recPosition singleRec;
                                    singleRec.row = currentRow;
                                    singleRec.columnStart = currentColumn;
                                    tempList.append(singleRec);
                            }else
                            {
                                continue ;
                            }
                        }

                }else
                {
                    break ;
                }
         }

         //save  list,use when look for str  in reccycle mode
         findResaultSave.clear();
         int tempListSize=tempList.count();
         for(int m=0;m<tempListSize;m++)
         {
             findResaultSave.append(tempList[m]);
         }

         int resaultCount=findResaultSave.count();
         if(resaultCount>0)
         {
             int  rowTemp=findResaultSave[0].row;
             int colTemp=findResaultSave[0].columnStart;
             index = model->index(rowTemp,colTemp);
             ui->tableView->setCurrentIndex(index);
             displaySearchPosSave=0;

             QString str3;
             str3 = QTime::currentTime().toString();
             str3= "cycle search find   "+str3;
             ui->label_3->setText(str3);
         }else
         {
             QString str3;
             str3 = QTime::currentTime().toString();
             str3= "cycle search not find   "+str3;
             ui->label_3->setText(str3);
         }

         qDebug()<<"tempList size is"<<tempListSize;
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    //cycle next
       if(findResaultSave.count()>0)
       {

            displaySearchPosSave=displaySearchPosSave+1;
            int max = findResaultSave.count()-1;
            int min = 0;
            if(displaySearchPosSave > max)
            {
                 displaySearchPosSave = max;
            }

            if(displaySearchPosSave < min)
            {
                displaySearchPosSave =min;
            }

            QAbstractItemModel *model = ui->tableView->model();
            QModelIndex index ;

            int  rowTemp=findResaultSave[displaySearchPosSave].row;
            int colTemp=findResaultSave[displaySearchPosSave].columnStart;
            index = model->index(rowTemp,colTemp);
            ui->tableView->setCurrentIndex(index);
       }
}

void MainWindow::on_pushButton_12_clicked()
{
    //cycle befor
       if(findResaultSave.count()>0)
       {

            displaySearchPosSave=displaySearchPosSave-1;
            int max = findResaultSave.count()-1;
            int min = 0;
            if(displaySearchPosSave > max)
            {
                 displaySearchPosSave = max;
            }

            if(displaySearchPosSave < min)
            {
                displaySearchPosSave =min;
            }

            QAbstractItemModel *model = ui->tableView->model();
            QModelIndex index ;

            int  rowTemp=findResaultSave[displaySearchPosSave].row;
            int colTemp=findResaultSave[displaySearchPosSave].columnStart;
            index = model->index(rowTemp,colTemp);
            ui->tableView->setCurrentIndex(index);
       }
}

void MainWindow::openFileSecond()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,"open file dialog","","");

    if(!fileName.isEmpty())
    {
        modelFileSecond->clear();
        setTableHeader();
        fileObject.setFileName(fileName);
        ui->label_2->setText(fileName);
        sizeSecond=0;
        fileObject.open(QIODevice::ReadOnly);
        sizeSecond=fileObject.size();
        fileObject.close();
       // fileList.clear();
        int m=0;
        int n=0;
         char *readByte=new char[1];//读取1字节
         fileObject.open(QIODevice::ReadOnly);
         QDataStream  dataStreamObject(&fileObject);
         QString str;
         for(int i=0;i<sizeSecond;i++)
         {
             fileObject.seek(i);
             dataStreamObject.readRawData(readByte,1);
             quint8 number=(quint8)readByte[0];
             n=i/16;
             m=i-n*16;

             str=QString("%1").arg(number, 2, 16, QLatin1Char('0')) ;
             modelFileSecond->setItem(n,m,new QStandardItem(str.toUpper()));
             modelFileSecond->item(n,m)->setBackground(QBrush(QColor(128,128,128)));

         }
        fileObject.close();
         rowSecond = n;
         columnEndAtSecond =m;
         delete readByte;
    }
}

void MainWindow::on_pushButton_13_clicked()
{  //open second file
    openFileSecond();
}

void MainWindow::on_pushButton_14_clicked()
{
    //compare diff
    if((size>0)&&(sizeSecond==size))
    {
            findResaultDiff.clear();

            QAbstractItemModel *model = ui->tableView->model();
            QAbstractItemModel *model2 = ui->tableView_2->model();
            QModelIndex index,index2 ;
            QString str,str2;
            quint8 hex,hex2;
             bool ok;
             recPosition single;
             QVariant data;
            for(int i=0;i<row;i++)
            {
                    for(int j=0;j<16;j++)
                    {
                            int pos = i*16+j;
                            if(pos < size )
                            {
                                     index = model->index(i,j);
                                     data = model->data(index);
                                     str=data.toString();
                                     hex = str.toInt(&ok, 16);       // hex == 255, ok == true

                                     index2 = model2->index(i,j);
                                     data = model2->data(index2);
                                     str2=data.toString();
                                     hex2 = str2.toInt(&ok, 16);       // hex == 255, ok == true
                                     if(hex!=hex2)
                                     {

                                         single.row=i;
                                         single.columnStart=j;
                                         findResaultDiff.append(single);
                                     }
                                ;
                            }
                            else
                            {
                                break ;
                            }
                    }
            }

            int diffCount = findResaultDiff.count();
            if(diffCount > 0)
            {
                QString strTemp = QString("%1 diff has find !").arg(diffCount, 0, 10) ;
                ui->label_3->setText(strTemp);
                displayDiffPos=0;
            }else
            {
                ui->label_3->setText("no diff !");
            }
    }

}

void MainWindow::printDiffResault()
{

}

void MainWindow::on_pushButton_15_clicked()
{   //diff  next
    if(findResaultDiff.count()>0)
    {

         displayDiffPos=displayDiffPos+1;
         int max = findResaultDiff.count()-1;
         int min = 0;
         if(displayDiffPos > max)
         {
              displayDiffPos = max;
         }

         if(displayDiffPos < min)
         {
             displayDiffPos =min;
         }

         QAbstractItemModel *model = ui->tableView->model();
         QAbstractItemModel *model2 = ui->tableView_2->model();
         QModelIndex index,index2 ;

         int  rowTemp=findResaultDiff[displayDiffPos].row;
         int colTemp=findResaultDiff[displayDiffPos].columnStart;
         index = model->index(rowTemp,colTemp);
         index2 = model2->index(rowTemp,colTemp);
         ui->tableView->setCurrentIndex(index);
         ui->tableView_2->setCurrentIndex(index2);
    }
}

void MainWindow::on_pushButton_16_clicked()
{  //diff before
    if(findResaultDiff.count()>0)
    {

         displayDiffPos=displayDiffPos-1;
         int max = findResaultDiff.count()-1;
         int min = 0;
         if(displayDiffPos > max)
         {
              displayDiffPos = max;
         }

         if(displayDiffPos < min)
         {
             displayDiffPos =min;
         }

         QAbstractItemModel *model = ui->tableView->model();
         QAbstractItemModel *model2 = ui->tableView_2->model();
         QModelIndex index,index2 ;

         int  rowTemp=findResaultDiff[displayDiffPos].row;
         int colTemp=findResaultDiff[displayDiffPos].columnStart;
         index = model->index(rowTemp,colTemp);
         index2 = model2->index(rowTemp,colTemp);
         ui->tableView->setCurrentIndex(index);
         ui->tableView_2->setCurrentIndex(index2);
    }
}

void MainWindow::on_pushButton_17_clicked()
{
     QString str = ui->lineEdit_3->text();
     qint64   hex  = str.toInt();
     str = QString("%1").arg(hex, 0, 16) ;
     str = str.toUpper();
     ui->lineEdit_3->setText(str);
}

void MainWindow::on_pushButton_18_clicked()
{
     this->close();
}

void MainWindow::on_pushButton_19_clicked()
{
    this->ui->pushButton_19->setText("中文界面");
    this->ui->pushButton_3->setText("打开文件1");
    this->ui->pushButton_8->setText("保存文件1");

    this->ui->pushButton_5->setText("查找");
    this->ui->pushButton_6->setText("下一个");
    this->ui->pushButton_7->setText("上一个");

    this->ui->pushButton_13->setText("打开文件2");
    this->ui->pushButton_9->setText("保存查找列表");
    this->ui->pushButton_10->setText("列表内查找");
    this->ui->pushButton_11->setText("上一个");
    this->ui->pushButton_12->setText("下一个");
    this->ui->pushButton_14->setText("进行文件比较");
    this->ui->pushButton_15->setText("不同下一个");
    this->ui->pushButton_16->setText("不同上一个");
    this->ui->pushButton_17->setText("转换为16进制");
    this->ui->pushButton_18->setText("退出程序");
}
