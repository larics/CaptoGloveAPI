#pragma once

#include "BoardTools/WearingPosition.h"
#include "BoardTools/ExternalSensor.h"

#ifdef __cplusplus
#include <unordered_map>

namespace GSdk {
	namespace BoardTools {
		/** \brief Assembly correspondings of sensor's positions */
		class ExternalSensorAssembly {
		private:
			std::unordered_map<int, int> m_tagsToIndexes;
			std::unordered_map<int, int> m_indexesToTags;

		public:
			/** \brief Returns all mounted tags of ExternaSensor for this assembly */
			std::vector<int> tags() const;

			/** \brief Returns all mounted index positions on board */
			std::vector<int> indexes() const;

			/** \brief Finds tag inside map. In case of missed tag returns -1. */
			int findTag(const int& index) const;

			/** \brief Finds index inside map. In case of missed index returns -1. */
			int findIndex(int tag) const;

			/** \brief Finds index inside map */
			int findIndex(const ExternalSensor& sensor) const;

			/** \brief Removes sntry tag-index by tag */
			void removeTag(const int& tag);

			/** \brief Removes sntry tag-index by index */
			void removeIndex(const int& index);

			/** \brief Pair tag and index */
			void pair(const int& index, const int& tag);

			/** \brief Custom equality operator comparing sensors content */
			bool operator==(const ExternalSensorAssembly& another) const;

			/** \brief Constructs this empty assembly */
			ExternalSensorAssembly();

			/** \brief Constructs this assembly for given position */
			ExternalSensorAssembly(const WearingPosition& position);
		};
	}
}

#endif
