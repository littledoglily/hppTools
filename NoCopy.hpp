/*Date:		2017/12/08 00:01:11
 *Author:	Jianlin LI
 */
#ifndef NOCOPY_
#define NOCOPY_
#include <iostream>
namespace multiTools{
class NoCopy {
protected:
	NoCopy(){}
	~NoCopy(){}
private:
	NoCopy(const NoCopy& );
	const NoCopy& operator = (const NoCopy& );
}; //NoCopy
} //multiTools
#endif
