//============================================================================
// Name        : CrossRoadObject.h
// Description : CrossRoadObject class for the roads in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_CROSSROADOBJECT_H
#define SE_PROJECT_CROSSROADOBJECT_H

#include <string>
#include <unordered_map>

#include "../../lib/utils/Id.h"
#include "../Object.h"

// Note: there are no pre/post- conditions because the parameter types solve all
// of them

/// Represents a crossroad
// ENSURE(properlyInitialized(), "Object is properly initialized");
class CrossRoadObject : public Object {
    const id counterPart;

  public:
    /**
     * Represents a crossroad \n
     * ENSURE(properlyInitialized(), "Object is properly initialized");
     * @param id_ id to given to the object
     * @param counterPart id second CrossRoadObject on crossing road
     */
    CrossRoadObject(const id id_, const id counterPart);

    id getCounterPart();
};

#endif  // SE_PROJECT_CROSSROADOBJECT_H
