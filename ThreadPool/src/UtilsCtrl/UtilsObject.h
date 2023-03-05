

#ifndef CGRAPH_UTILSOBJECT_H
#define CGRAPH_UTILSOBJECT_H

#include "UtilsDefine.h"

CGRAPH_NAMESPACE_BEGIN

class UtilsObject : public CObject {

public:
	CStatus run() override {
		CGRAPH_NO_SUPPORT
	}
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSOBJECT_H
