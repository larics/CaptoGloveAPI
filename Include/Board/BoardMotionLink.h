#pragma once

#include "Base/Vector3.h"
#include "Board/AxisMode.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * \brief Settings for customization mouse/joystick axis.
 * Contains methods for ebabling/disabling axis as well as setting
 * scaling factors and so on.
 */
#pragma pack (push, 1)
typedef struct {
    /** \brief Is this axis link enabled.*/
    bool isEnabled;
    
    /**
     * \brief This motion axis, vector of floats.
     * Relates to phisical axis orientation of device.
     */
    GSdkVector3f realAxis;
    
    /**
     * \brief Manupulating virtual element. Purpose can differs a lot
     * depending from axis usage (like mouse button or joystick button).
     */
    unsigned char virtualAxis;
    
    /**
     * \brief Scale for given axis, lays in range from 0 to 1.
     * Larger values leads to more sensibility.
     */
    float factor;
    
    /** \brief axis mode. For description @see AxisMode. */
    unsigned char axisMode;
} GSdkBoardMotionLink;
#pragma pack (pop)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace GSdk {
	namespace Board {
		using namespace Base;

        typedef GSdkBoardMotionLink BoardMotionLink;
        
        /** \brief Generates motion link with default params */
        BoardMotionLink getBoardMotionLink(const bool &isEnabled = false,
                                           const Vector3f &realAxis = GSdkVector3fZero,
                                           const unsigned char &virtualAxis = 0,
                                           const float &factor = 1,
                                           const unsigned char &axisMode = GSdkAxisModeCircular);
	}
}

/** \brief Returns string representation of this motion link. */
std::string getString(const GSdk::Board::BoardMotionLink &value);

#endif
