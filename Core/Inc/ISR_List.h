/*
 * ISR_HAL.h
 *
 *  Created on: 05-Jul-2026
 *      Author: ABU MUSA TRADERS
 */

#ifndef INC_ISR_HAL_H_
#define INC_ISR_HAL_H_
#include "main.h"
#include <vector>
#include <algorithm>
using namespace std;
template<class T>
 class ISR{
public:
	ISR(void){
	ISR_LIST.clear();
	}
  ~ISR(void){
	ISR_LIST.clear();
	}

	void add(T *obj){
	ISR_LIST.push_back(obj);
	}
	void remove(T *obj){
		if(ISR_LIST.size()==0){
			return;
		}
		ISR_LIST.erase(find(ISR_LIST.begin(),ISR_LIST.end(),obj));
	}
		T* get(uint16_t index){
			if(ISR_LIST.size()==0){
				return NULL;
			}
			return ISR_LIST[index];
		}

		size_t size(void){
			return ISR_LIST.size();
		}




private:
		vector<T*> ISR_LIST;
};

#endif /* INC_ISR_HAL_H_ */
