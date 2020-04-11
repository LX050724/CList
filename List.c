#include "List.h"

#ifdef __cplusplus
extern "C" {
#endif

	List_StatusTypeDef List_Creat(List_HandleTypeDef* List_Handle, size_t Size)
	{
		if (Size)List_Handle->Size = Size;
		else return List_ERROR;
		List_Handle->New = (Nodep)List_malloc(sizeof(Node));
		if (List_Handle->New == NULL)return List_ERROR;
		else
		{
			List_memset(List_Handle->New, 0, sizeof(Node));
			List_Handle->New->Data = List_malloc(Size);
			if (List_Handle->New->Data)List_memset(List_Handle->New->Data, 0, Size);
			else return List_ERROR;
			return List_OK;
		}
	}

	List_StatusTypeDef List_empty(List_HandleTypeDef* List_Handle)
	{
		if (List_Handle->Len)return List_ERROR;
		else return List_OK;
	}

	List_StatusTypeDef List_resize(List_HandleTypeDef* List_Handle, size_t Len)
	{
		while (List_Handle->Len > Len)
		{
			if (List_pop_back(List_Handle) == List_ERROR)
				return List_ERROR;
		}
		return List_OK;
	}

	List_StatusTypeDef List_copy(List_HandleTypeDef* Dst_List_Handle, List_HandleTypeDef* Src_List_Handle)
	{
		if (Dst_List_Handle->Len != 0)return List_ERROR;
		List_iteratorTypeDef it;
		List_iterator_Init(&it, Src_List_Handle);
		do {
			if (!List_push_back(Dst_List_Handle, List_iterator_read(&it)))return List_ERROR;
		} while (List_iterator_mov_back(&it));
		return List_OK;
	}

	List_StatusTypeDef List_iterator_erase(List_iteratorTypeDef* List_iterator)
	{
		List_StatusTypeDef fb = List_OK;
		if (List_iterator->Using)
		{
			if (List_iterator->Using == List_iterator->Handle->Head)
			{
				if (!List_pop_front(List_iterator->Handle))return List_ERROR;
				if (!List_iterator_mov_head(List_iterator))return List_ERROR;
			}
			else if (List_iterator->Using == List_iterator->Handle->End)
			{
				if (!List_pop_back(List_iterator->Handle))return List_ERROR;
				if (!List_iterator_mov_end(List_iterator))return List_ERROR;
			}
			else
			{
				Nodep temp = List_iterator->Using;
				List_iterator->Using->Last->Next = List_iterator->Using->Next;
				List_iterator->Using->Next->Last = List_iterator->Using->Last;
				List_iterator->Using = List_iterator->Using->Next;
				List_iterator->Handle->Len--;
				List_free(temp->Data);
				List_free(temp);
				return List_OK;
			}
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_iterator_erase_area(List_iteratorTypeDef* List_iterator1, List_iteratorTypeDef* List_iterator2)
	{
		if (List_iterator1->Handle != List_iterator2->Handle)return List_ERROR;
		if (List_iterator1->Locat > List_iterator2->Locat)
		{
			List_iteratorTypeDef* tmp = List_iterator1;
			List_iterator1 = List_iterator2;
			List_iterator2 = tmp;
		}
		if (List_iterator1->Locat == List_iterator2->Locat)return List_iterator_erase(List_iterator1);
		do {
			if (!List_iterator_erase(List_iterator1))return List_ERROR;
		} while (List_iterator1->Using != List_iterator2->Using);
		if (!List_iterator_erase(List_iterator1))return List_ERROR;
		return List_OK;
	}

	List_StatusTypeDef List_remove_if(List_HandleTypeDef* List_Handle, unsigned char(*CallBack)(void*))
	{
		List_iteratorTypeDef it;
		if (!List_iterator_Init(&it, List_Handle))return List_ERROR;
		do {
			if (CallBack(List_iterator_read(&it)))
				List_iterator_erase(&it);
		} while (List_iterator_mov_back(&it));
		return List_OK;
	}

	List_StatusTypeDef List_iterator_Init(List_iteratorTypeDef* List_iterator, List_HandleTypeDef* List_Handle)
	{
		List_iterator->Locat = 0;
		if (List_Handle)List_iterator->Handle = List_Handle;
		else
		{
			List_iterator->Handle = NULL;
			return List_ERROR;
		}
		if (List_Handle->Head)List_iterator->Using = List_Handle->Head;
		else
		{
			List_iterator->Using = NULL;
			return List_ERROR;
		}
		return List_OK;
	}

	List_StatusTypeDef List_iterator_insert(List_iteratorTypeDef* List_iterator, void* Data)
	{
		if (Data == NULL)return List_ERROR;
		if (List_iterator->Using == List_iterator->Handle->End)return List_push_back(List_iterator->Handle, Data);
		List_memcpy(List_iterator->Handle->New->Data, Data, List_iterator->Handle->Size);
		Nodep temp = List_iterator->Using->Next;
		List_iterator->Using->Next = List_iterator->Handle->New;
		List_iterator->Handle->New->Last = List_iterator->Using;
		List_iterator->Handle->New->Next = temp;
		temp->Last = List_iterator->Handle->New;
		return List_Creat(List_iterator->Handle, List_iterator->Handle->Size);
	}

	void* List_read_front(List_HandleTypeDef* List_Handle)
	{
		if (List_Handle->Len == 0)return NULL;
		else
		{
			return List_Handle->Head->Data;
		}
	}

	void* List_read_back(List_HandleTypeDef* List_Handle)
	{
		if (List_Handle->Len == 0)return NULL;
		else
		{
			return List_Handle->End->Data;
		}
	}

	void* List_iterator_read(List_iteratorTypeDef* List_iterator)
	{
		if (List_iterator->Handle->Len == 0)return NULL;
		if (List_iterator->Using == NULL)
		{
			if (List_iterator_mov_head(List_iterator) == List_ERROR)return NULL;
		}
		if (List_iterator->Using != NULL)
			return List_iterator->Using->Data;
		else __assume(0);
	}

	List_StatusTypeDef List_iterator_mov_front(List_iteratorTypeDef* List_iterator)
	{
		if (List_iterator->Using == NULL)return List_ERROR;
		if (List_iterator->Using->Last)
		{
			List_iterator->Using = List_iterator->Using->Last;
			List_iterator->Locat--;
			return List_OK;
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_iterator_mov_back(List_iteratorTypeDef* List_iterator)
	{
		if (List_iterator->Using == NULL)return List_ERROR;
		if (List_iterator->Using->Next)
		{
			List_iterator->Using = List_iterator->Using->Next;
			List_iterator->Locat++;
			return List_OK;
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_iterator_mov_head(List_iteratorTypeDef* List_iterator)
	{
		if (List_iterator->Handle->Head)
		{
			List_iterator->Using = List_iterator->Handle->Head;
			List_iterator->Locat = 0;
			return List_OK;
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_iterator_mov_end(List_iteratorTypeDef* List_iterator)
	{
		if (List_iterator->Handle->End)
		{
			List_iterator->Using = List_iterator->Handle->End;
			List_iterator->Locat = List_iterator->Handle->Len - 1;
			return List_OK;
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_iterator_mov_goto(List_iteratorTypeDef* List_iterator, size_t locat)
	{
		if (locat < List_iterator->Locat)
		{
			do {
				if (!List_iterator_mov_front(List_iterator))return List_ERROR;
			} while (locat < List_iterator->Locat);
		}
		else if (locat > List_iterator->Locat)
		{
			do {
				if (!List_iterator_mov_back(List_iterator))return List_ERROR;
			} while (locat > List_iterator->Locat);
		}
		return List_OK;
	}

	List_StatusTypeDef List_push_front(List_HandleTypeDef* List_Handle, void* Data)
	{
		if (List_Handle->New)
		{
			if (Data)List_memcpy(List_Handle->New->Data, Data, List_Handle->Size);//写入新元素
			else return List_ERROR;
			if (List_Handle->Len == List_ERROR)
			{//如果是空链表，写入首元素尾元素
				List_Handle->Head = List_Handle->New;
				List_Handle->End = List_Handle->New;
			}
			else
			{//链表非空
				List_Handle->New->Next = List_Handle->Head;						//新元素下一个指针指向当前首元素
				List_Handle->Head = List_Handle->New;									//更新首元素
				List_Handle->Head->Next->Last = List_Handle->Head;		//连接第二给元素的上一个元素
			}
			List_Handle->Len++;
			return List_Creat(List_Handle, List_Handle->Size);
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_push_back(List_HandleTypeDef* List_Handle, void* Data)
	{
		if (List_Handle->New)
		{
			if (Data)List_memcpy(List_Handle->New->Data, Data, List_Handle->Size);
			else return List_ERROR;
			if (List_Handle->Len == 0)
			{
				List_Handle->Head = List_Handle->New;
				List_Handle->End = List_Handle->New;
			}
			else
			{
				List_Handle->New->Last = List_Handle->End;		//新元素上一个指向尾元素
				List_Handle->End->Next = List_Handle->New;		//尾元素下一个指向新元素
				List_Handle->End = List_Handle->New;					//更新尾元素
			}
			List_Handle->Len++;
			return List_Creat(List_Handle, List_Handle->Size);
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_pop_front(List_HandleTypeDef* List_Handle)
	{
		Nodep temp;
		if (List_Handle->Head)
		{
			temp = List_Handle->Head;
			if (List_Handle->Head->Next)List_Handle->Head = List_Handle->Head->Next;	//首元素后移
			List_Handle->Head->Last = NULL;									//首元素前一个指针清空
			List_free(temp->Data);
			List_free(temp);
			List_Handle->Len--;
			return List_OK;
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_pop_back(List_HandleTypeDef* List_Handle)
	{
		Nodep temp;
		if (List_Handle->End)
		{
			temp = List_Handle->End;
			if (List_Handle->End->Last)List_Handle->End = List_Handle->End->Last;	//尾元素前移
			List_Handle->End->Next = NULL;								//尾元素下一个指针清空
			List_free(temp->Data);
			List_free(temp);
			List_Handle->Len--;
			return List_OK;
		}
		return List_ERROR;
	}

	List_StatusTypeDef List_Delete(List_HandleTypeDef* List_Handle)
	{
		while (List_Handle->Len)
		{
			if (!List_pop_front(List_Handle))return List_ERROR;
		}
		List_free(List_Handle->New->Data);
		List_free(List_Handle->New);
		List_memset(List_Handle, 0, sizeof(List_HandleTypeDef));
		return List_OK;
	}

#ifdef __cplusplus
}
#endif
