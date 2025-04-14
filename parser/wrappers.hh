/*
 * wrappers.hh
 *
 *  Created on: Mar 30, 2025
 *      Author: aleksandr
 */

#ifndef WRAPPERS_HH_
#define WRAPPERS_HH_

#include <variant>
using namespace std;
namespace common_parsing{
struct None{};
template<typename T>
struct Maybe{
	std::variant<None,T> val;
	Maybe():val(None{}){};
	Maybe(T _val):val(_val){};
	bool isEmpty(){
		return val.index()==0;
	}
    T get_ok(){
        return get<T>(val);
    }
};
}
#endif /* WRAPPERS_HH_ */
