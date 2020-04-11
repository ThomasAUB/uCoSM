#pragma once




#include <tuple>



template<class ...ModuleCollection> 
class ModuleKit_M
{

	using items_t = std::tuple<ModuleCollection...>;
	items_t mItemModules;

public:

	template<typename T>
	T& get()
	{
		return std::get<T>(mItemModules);
	}

	template<typename T, size_t I = 0>
	void init(T *t)
	{
		std::get<I>(mItemModules).init(t);
		if constexpr(I+1 != std::tuple_size<items_t>::value)
		    init<T, I+1>(t);
	}

	template<typename T, size_t I = 0>
	bool isExeReady(T *t)
	{
		if constexpr (I+1 != std::tuple_size<items_t>::value){
			if(std::get<I>(mItemModules).isExeReady(t)){
				return isExeReady<T, I+1>(t);
			}else{
				return false;
			}
		}else{
			return std::get<I>(mItemModules).isExeReady(t);
		}
	}
	
	template<typename T, size_t I = 0>
	bool isDelReady(T *t) {
		if constexpr (I+1 != std::tuple_size<items_t>::value){
			if(std::get<I>(mItemModules).isDelReady(t)){
				return isDelReady<T, I+1>(t);
			}else{
				return false;
			}
		}else{ 
			return std::get<I>(mItemModules).isDelReady(t);
		}
	}

	template<typename T, size_t I = 0>
	void makePreExe(T *t) {
		std::get<I>(mItemModules).makePreExe(t);
		if constexpr(I+1 != std::tuple_size<items_t>::value)
		    makePreExe<T, I+1>(t);
	}

	template<typename T, size_t I = 0>
	void makePostExe(T *t) {
		std::get<I>(mItemModules).makePostExe(t);
		if constexpr(I+1 != std::tuple_size<items_t>::value)
		    makePostExe<T, I+1>(t);
	}
	
	template<typename T, size_t I = 0>
	void makePreDel(T *t) {
		std::get<I>(mItemModules).makePreDel(t);
		if constexpr(I+1 != std::tuple_size<items_t>::value)
		    makePreDel<T, I+1>(t);
	}
};
