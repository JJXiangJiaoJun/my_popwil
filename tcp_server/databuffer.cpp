#include "databuffer.h"
#include "cstring"
#include <QException>
DataBuffer::DataBuffer()
{
    m_nBufferSize = BUFFER_MAX_SIZE;
    m_nOffset = 0;
    m_pBuffer = new char[m_nBufferSize];
    memset(m_pBuffer,0,sizeof(m_pBuffer));
}


DataBuffer::~DataBuffer()
{
    delete [] m_pBuffer;
    m_pBuffer = NULL;
    m_nBufferSize = 0;
    m_nOffset = 0;
}

/**
 * @brief DataBuffer::getDataLen
 * 获得缓冲区中数据大小
 * @return 缓冲区中数据大小
 */
int DataBuffer::getDataLen()
{
    return m_nOffset;
}

/**
 * @brief DataBuffer::reBufferSize
 * @param nLen
 * @return
 */
bool DataBuffer::reBufferSize(int nLen)
{
    char *oBuffer = m_pBuffer;  //保存原缓冲区地址
    try
    {
        nLen = nLen<64?64:nLen;
        while(m_nBufferSize<nLen)
        {
            m_nBufferSize *=2;
        }
        m_pBuffer = new char[m_nBufferSize];  //分配新缓冲区
        memset(m_pBuffer,0,sizeof(m_pBuffer));
        memcpy(m_pBuffer,oBuffer,m_nOffset);
        delete []oBuffer;
    }
    catch(QException e)
    {
        return false;
    }
    return true;

}


/**
 * @brief DataBuffer::addMsg
 * @param pBuf 需要添加的内容
 * @param nLen 数据长度
 * @return 成功返回true，失败返回false
 */
bool DataBuffer::addMsg(char *pBuf, int nLen)
{
    try{
        if(m_nOffset + nLen>m_nBufferSize)      //如果缓冲区过小，则重新调整其大小
            reBufferSize(m_nOffset + nLen);
        memcpy(m_pBuffer + m_nOffset,pBuf,nLen);
        m_nOffset += nLen;                      //修改数据偏移
    }
    catch(QException e)
    {
        return false;
    }
    return true;
}

/**
 * @brief DataBuffer::reset
 */

void DataBuffer::reset()
{
    if(m_nOffset > 0)
    {
        memset(m_pBuffer, 0, sizeof(m_pBuffer));
        m_nOffset = 0;
    }
}


void DataBuffer::poll(int nLen)
{
    if(m_nOffset == 0 || m_pBuffer == NULL)
        return;

   if(m_nOffset >= nLen)
   {
        memcpy(m_pBuffer, m_pBuffer + nLen, m_nOffset - nLen);
        m_nOffset -= nLen;
   }
}

