#ifndef _S5_BUFFER_H_
#define _S5_BUFFER_H_
/**
 * Copyright (C), 2019.
 *
 * @file
 * һ��buffer����һ���������ڴ档һ�������һ��ָ�����һ�����ȾͿ�������һ���ڴ档��ʹ��RDMA����ʱ
 * ����ڴ���Ҫ�������Ϣ����local key, remote key, offset���ҶԲ�ͬ��RDMA�豸��ע�����и��Զ�Ӧ
 * ��local key, remote key������������ͬһ��buffer������ͬ���豸ʹ�þ�Ҫ�ṩ��ͬ�ķ���Ҫ�ء������
 * buffer_descriptor ���ڵ����塣buffer_descriptor�����¼�����buffer���ֳ����µķ�����Ϣ��
 *
 * buffer �ᰴ����󳤶ȷ��䣬����������������IO��64K byte�� ��ôbufer�������64K byte��Ҳ����buf_size
 * ��65536��Ȼ��������һ��4K byte IO����ʱ����Ч���ݵĳ���,��data_len��4096
 */

#include "s5_fixed_size_queue.h"

class BufferPool;
class S5Connection;

struct BufferDescriptor
{
	void* buf;
	int data_len; /// this is the validate data len in the buffer.
	int(*on_work_complete)(BufferDescriptor* bd, S5Connection* conn, void* cbk_data);
	void* cbk_data;
	int buf_size; /// this is the size, i.e. max size of buf
	BufferPool* owner_pool;
};

class BufferPool
{
public:
	int init(size_t buffer_size, size_t count);
	BufferDescriptor* alloc();
	int free(BufferDescriptor* bd);
private:
	S5FixedSizeQueue<BufferDescriptor*> free_bds;
	void* data_buf;
};
#endif //_S5_BUFFER_H_

