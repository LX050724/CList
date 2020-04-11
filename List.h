#ifndef _C_List_
#define _C_List_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define List_malloc malloc
#define List_memset memset
#define List_memcpy memcpy
#define List_free free

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum List_Status {
		List_ERROR = 0,
		List_OK,
	}List_StatusTypeDef;

	typedef struct Node {
		void* Data;
		struct Node* Next;
		struct Node* Last;
	}Node, * Nodep;

	typedef struct {
		Nodep Head;
		Nodep End;
		Nodep New;
		size_t Len;
		size_t Size;
	}List_HandleTypeDef;

	typedef struct {
		List_HandleTypeDef* Handle;
		Nodep Using;
		size_t Locat;
	}List_iteratorTypeDef;

	/**
	* @brief  初始化链表
	* @param  List_Handle链表句柄
	* @param  Size元素数据内存大小
	* @retval List status
	*/
	List_StatusTypeDef List_Creat(List_HandleTypeDef* List_Handle, size_t Size);

	/**
	* @brief  判断链表是否为空
	* @param  List_Handle链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_empty(List_HandleTypeDef* List_Handle);

	/**
	* @brief  将长度改为只容纳n个元素
	* @param  List_Handle链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_resize(List_HandleTypeDef* List_Handle, size_t Len);

	/**
	* @brief  链表拷贝
	* @param  Dst_List_Handle目标链表句柄
	* @param  Src_List_Handle源链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_copy(List_HandleTypeDef* Dst_List_Handle, List_HandleTypeDef* Src_List_Handle);

	/**
	* @brief  容器中的元素数
	* @param  Dst_List_Handle目标链表句柄
	* @retval 容器中的元素数
	*/
#define __List_size(List_Handle) (List_Handle.Len)

	/**
	* @brief  移除迭代器所指元素
	* @param  List_iterator迭代器
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_erase(List_iteratorTypeDef* List_iterator);

	/**
	* @brief  移除两迭代器范围内元素，包括两迭代器所指元素
	* @param  List_iterator1迭代器1
	* @param  List_iterator2迭代器2
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_erase_area(List_iteratorTypeDef* List_iterator1, List_iteratorTypeDef* List_iterator2);

	/**
	* @brief  按条件移除链表中元素
	* @param  List_Handle链表句柄
	* @param  CallBack判断回调函数返回1删除，0保留
	* @retval List status
	*/
	List_StatusTypeDef List_remove_if(List_HandleTypeDef* List_Handle, unsigned char(*CallBack)(void*));

	/**
	* @brief  初始化迭代器
	* @param  List_iterator迭代器
	* @param  List_Handle链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_Init(List_iteratorTypeDef* List_iterator, List_HandleTypeDef* List_Handle);

	/**
	* @brief  在迭代器位置后插入元素
	* @param  List_iterator迭代器
	* @param  Data元素指针
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_insert(List_iteratorTypeDef* List_iterator, void* Data);

	/**
	* @brief  在迭代器位置后插入元素
	* @param  List_iterator迭代器
	* @param  Data元素
	* @retval List status
	*/
#define __List_iterator_insert(List_iterator,Data) List_iterator_insert(&(List_iterator),(void*)&(Data))

	/**
	* @brief  带类型初始化链表
	* @param  List_Handle链表句柄
	* @param  Type类型
	* @retval List status
	*/
#define __List_Creat(List_Handle,Type) List_Creat((&List_Handle),sizeof(Type))

	/**
	* @brief  返回链表首元素的数据
	* @param  List_Handle链表句柄
	* @retval 数据指针
	*/
	void* List_read_front(List_HandleTypeDef* List_Handle);

	/**
	* @brief  返回链表首元素的数据
	* @param  List_Handle链表句柄
	* @param  Type类型
	* @retval 数据
	*/
#define __List_read_front(List_Handle,Type) (*((Type*)List_read_front(&List_Handle)))

	/**
	* @brief  返回链表尾元素的数据指针
	* @param  List_Handle链表句柄
	* @retval 数据指针
	*/
	void* List_read_back(List_HandleTypeDef* List_Handle);

	/**
	* @brief  返回链表尾元素的数据
	* @param  List_Handle链表句柄
	* @param  Type类型
	* @retval 数据
	*/
#define __List_read_back(List_Handle,Type) (*((Type*)List_read_back(&List_Handle)))

	/**
	* @brief  返回迭代器所指元素数据指针
	* @param  List_iterator迭代器指针
	* @retval 数据指针
	*/
	void* List_iterator_read(List_iteratorTypeDef* List_iterator);

	/**
	* @brief  返回迭代器所指元素数据
	* @param  List_iterator迭代器
	* @param  Type类型
	* @retval 数据
	*/
#define __List_iterator_read(List_iterator,Type) (*((Type*)List_iterator_read(&List_iterator)))

	/**
	* @brief  向前移动迭代器
	* @param  List_iterator迭代器
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_mov_front(List_iteratorTypeDef* List_iterator);

	/**
	* @brief  向后移动迭代器
	* @param  List_iterator迭代器
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_mov_back(List_iteratorTypeDef* List_iterator);

	/**
	* @brief  移动迭代器至首元素
	* @param  List_iterator迭代器
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_mov_head(List_iteratorTypeDef* List_iterator);

	/**
	* @brief  移动迭代器至尾元素
	* @param  List_iterator迭代器
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_mov_end(List_iteratorTypeDef* List_iterator);

	/**
	* @brief  移动迭代器至指定元素
	* @param  List_iterator迭代器
	* @param  locat在链表中的位置
	* @retval List status
	*/
	List_StatusTypeDef List_iterator_mov_goto(List_iteratorTypeDef* List_iterator, size_t locat);

	/**
	* @brief  移动迭代器指定偏移量
	* @param  List_iterator迭代器
	* @param  offset偏移量
	* @retval List status
	*/
#define __List_iterator_mov_offset(List_iterator,offset) ((List_iterator).Locat + offset) > 0 ? List_iterator_mov_goto(&(List_iterator),(List_iterator).Locat + offset):List_ERROR

	/**
	* @brief  返回迭代器所在的位置
	* @param  List_iterator迭代器
	* @retval Locat
	*/
#define __List_iterator_return_locat(List_iterator) (List_iterator.Locat)

	/**
	* @brief  添加元素到链表头
	* @param  List_Handle链表句柄
	* @param  Data数据指针
	* @retval List status
	*/
	List_StatusTypeDef List_push_front(List_HandleTypeDef* List_Handle, void* Data);

	/**
	* @brief  添加元素到链表头
	* @param  List_Handle链表句柄
	* @param  Data数据
	* @retval List status
	*/
#define __List_push_front(List_Handle,Data) List_push_front(&(List_Handle),(void*)(Data))

	/**
	* @brief  添加元素到链表尾
	* @param  List_Handle链表句柄
	* @param  Data数据指针
	* @retval List status
	*/
	List_StatusTypeDef List_push_back(List_HandleTypeDef* List_Handle, void* Data);

	/**
	* @brief  添加元素到链表尾
	* @param  List_Handle链表句柄
	* @param  Data数据
	* @retval List status
	*/
#define __List_push_back(List_Handle,Data) List_push_back(&(List_Handle),(void*)&(Data))

	/**
	* @brief  移除首元素
	* @param  List_Handle链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_pop_front(List_HandleTypeDef* List_Handle);

	/**
	* @brief  移除尾元素
	* @param  List_Handle链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_pop_back(List_HandleTypeDef* List_Handle);

	/**
	* @brief  删除链表
	* @param  List_Handle链表句柄
	* @retval List status
	*/
	List_StatusTypeDef List_Delete(List_HandleTypeDef* List_Handle);


#ifdef __cplusplus
}
#endif

#endif
