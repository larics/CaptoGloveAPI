#pragma once

#include "BoardTools/BoardUserInfo.h"
#include "Base/AssemblyTypes.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BoardTools {
		using namespace Base;

		typedef Result<BoardUserInfo> BoardUserInfoResult;

		enum BoardUserInfoError {
			BoardUserInfoErrorIncorrectSize = 1,
			BoardUserInfoErrorUnexpectedHeader = 2,
			BoardUserInfoErrorUnexpectedTrailer = 3,
		};

		/** \brief Basic class for encoding and decoding user info to binary stream */
		class BoardUserInfoCoder {
		public:
			BoardUserInfoCoder();

			/** \brief Decodes bytes to user info */
			BoardUserInfoResult decode(const std::vector<unsigned char>& bytes);

			/** \brief Decodes bytes to user info, otherwise returns default value */
			BoardUserInfo decodeOrDefault(const std::vector<unsigned char>& bytes, const WearingPosition& position);

			/** \brief Encodes bytes from user info */
			std::vector<unsigned char> encode(const BoardUserInfo& userInfo);
		};
	}
}
#endif
