#pragma once

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif 
	/** \brief Color structure. Every component has value from 0 to 255. */
#pragma pack (push, 1)
	typedef struct {
		/** \brief Red component (0..255) */
		unsigned char red;

		/** \brief Green component (0..255) */
		unsigned char green;

		/** \brief Blue component (0..255) */
		unsigned char blue;
	} GSdkBoardColor;
#pragma pack (pop)

	/** \brief Construct black color */
	extern GSdkBoardColor const GSdkBoardColorBlack;
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkBoardColor BoardColor;
	}
}

/** \brief Equality for color */
bool operator == (const GSdkBoardColor& lhs, const GSdkBoardColor& rhs);

/** \brief Non equality for color */
bool operator != (const GSdkBoardColor& lhs, const GSdkBoardColor& rhs);

/** \brief Description of this color */
std::string getString(const GSdkBoardColor &value);

#endif
