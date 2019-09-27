#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <QtGlobal>
#include <QByteArray>
#define BUFFER_MAX_SIZE 2048

/**
 * @brief The DataBuffer class
 * TCP服务器接收缓冲区
 */
class DataBuffer
{
public:
    char *m_pBuffer;          //缓冲区
    QByteArray tcp_buffer;
    qint32  m_nBufferSize;      //缓冲区大小
    qint32  m_nOffset;          //缓冲区当前数据大小

    int getDataLen();           //获取缓冲区中数据大小
    bool reBufferSize(int nLen); //调整缓冲区大小
    bool addMsg(char *pBuf,int nLen);   //添加消息到缓冲区
    void reset();                       //缓冲区复位
    void poll(int nLen);                //移除缓冲区收不的第一个数据包


public:

    DataBuffer();
    ~DataBuffer();
};

#endif // DATABUFFER_H
