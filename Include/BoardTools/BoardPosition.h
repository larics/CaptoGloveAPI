#pragma once

#ifdef __cplusplus
namespace GSdkCxxBoardTools {
	/**
	* \brief Description of board displacement
	*/
	typedef enum {
		/** \brief Board is mounting as left sensor (on user shoulder) */
		BoardPositionLeftSensor,

		/** \brief Board is mounting as left glove */
		BoardPositionLeftGlove,

		/** \brief Board is mounting as right sensor (on user shoulder) */
		BoardPositionRightSensor,

		/** \brief Board is mounting as right glove */
		BoardPositionRightGlove,
	} BoardPosition;
}
#endif
