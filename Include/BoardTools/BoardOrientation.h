#pragma once

#ifdef __cplusplus
namespace GSdk {
	namespace BoardTools {
		/**
		* \brief User defined weared orientation of board or other item
		*/
		typedef enum {
			/** \brief User holds it by top cap to ceil */
			BoardOrientationCapToCeil = 0,

			/** \brief User holds it by top cap to right or left */
			BoardOrientationCapToWall = 1,
		} BoardOrientation;
	}
}
#endif
