#pragma once

#include "BoardTools/WearingPosition.h"
#include "BoardTools/BoardOrientation.h"
#include "BoardTools/ExternalSensorAssembly.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BoardTools {
		/** \brief Basic characteristics about board usage scenarious */
		class BoardUserInfo {
		private:
			WearingPosition m_position;
			BoardOrientation m_orientation;
			ExternalSensorAssembly m_sensorAssembly;

		public:
			/** \brief Board body orientation of this board. */
			const WearingPosition& position() const;

			/** \brief Used hand orientation of this board. */
			const BoardOrientation& orientation() const;

			/** \brief Sensors map, used to configure this sensor */
			const ExternalSensorAssembly& sensorAssembly() const;

			/** \brief Custom equality operator comparing content */
			bool operator==(const BoardUserInfo& another) const;

			/** \brief Constructs given sensor assembly with default parameters. */
			BoardUserInfo(const WearingPosition& position,
				const BoardOrientation& orientation,
				const ExternalSensorAssembly& sensorAssembly);

			/**
			 * \brief Provides default user info for board position. It includes
			 * preferred sensor mounting for this position as well as default
			 * orientation.
			 */
			BoardUserInfo(const WearingPosition& position);
		};
	}
}
#endif
