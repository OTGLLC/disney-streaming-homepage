#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#ifdef _DEBUG
#define OTG_LOG(x) std::cout<<x<<std::endl




#else
#define OTG_LOG(x)


#endif

#endif
