
#ifndef CGRAPH_UQUEUEOBJECT_H
#define CGRAPH_UQUEUEOBJECT_H

#include <mutex>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

class UQueueObject : public UThreadObject {
protected:
	std::mutex mutex_;
	std::condition_variable cv_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UQUEUEOBJECT_H
