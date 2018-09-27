///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2016 Advanced Software Engineering Limited
//
// You may use and modify the code in this file in your application, provided the code and
// its modifications are used only in conjunction with ChartDirector. Usage of this software
// is subjected to the terms and condition of the ChartDirector license.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef QDOUBLEBUFFEREDQUEUE_HDR
#define QDOUBLEBUFFEREDQUEUE_HDR

#include <QMutex>

template <class T>
class QDoubleBufferedQueue
{
public:
    //
    // 构造函数-构造一个给定数组大小的双缓冲数组
    //
    QDoubleBufferedQueue(int bufferSize = 10000) :
        bufferLen(0), bufferSize(bufferSize)
	{ 
		buffer0 = buffer = new T[bufferSize]; 
		buffer1 = new T[bufferSize];
	}

    //
    // 析构函数
    //
    ~QDoubleBufferedQueue()
	{
		 delete[] buffer0;
		 delete[] buffer1;
	}

    //队列中加入一个元素
    // 如果成功返回True，失败则返回False
    //
	bool put(const T& datum)
	{
        mutex.lock();//mutex是互斥量，把buffer已经指针锁起来，不允许其他现成进行操作
		bool canWrite = bufferLen < bufferSize;
		if (canWrite) buffer[bufferLen++] = datum;
        mutex.unlock();
		return canWrite;
	}

    // 从队列中获取所有元素, data指针即为二级指针，此步操作之后，*data变为数据指针,返回值为数组大小
    //
	int get(T** data)
	{
        mutex.lock();
        *data = buffer;
		int ret = bufferLen;
        buffer = (buffer == buffer0) ? buffer1 : buffer0;//交换
		bufferLen = 0;
        mutex.unlock();
		return ret;
	}

private:

    // 禁止复制
    QDoubleBufferedQueue & operator=(const QDoubleBufferedQueue&);
    QDoubleBufferedQueue(const QDoubleBufferedQueue&);

	T* buffer0;
	T* buffer1;
	T* buffer;
    int bufferLen;//当前指针
    int bufferSize;//缓冲区总的长度
    QMutex mutex;
};


#endif
