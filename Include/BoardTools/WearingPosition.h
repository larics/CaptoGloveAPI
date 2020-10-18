#pragma once

/**
 * \brief Description of board displacement
 */
typedef enum {
  /** \brief Board is mounting as left sensor (on user shoulder) */
  GSdkWearingPositionLeftSensor,

  /** \brief Board is mounting as left glove */
  GSdkWearingPositionLeftGlove,

  /** \brief Board is mounting as right sensor (on user shoulder) */
  GSdkWearingPositionRightSensor,

  /** \brief Board is mounting as right glove */
  GSdkWearingPositionRightGlove,
} GSdkWearingPosition;

#ifdef __cplusplus
namespace GSdk {
	namespace BoardTools {
		typedef GSdkWearingPosition WearingPosition;
	}
}
#endif
