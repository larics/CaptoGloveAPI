#pragma once

#ifdef __cplusplus
#include <memory>
#include <string>
#include <vector>
#include <cstring>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GSDK_MAX_RESULT_DESCRIPTION_LENGTH  96

	/** \brief Basic result of any operation */
#pragma pack (push, 1)
	typedef struct {
		int status;
		int domain;
		char description[GSDK_MAX_RESULT_DESCRIPTION_LENGTH];
	} GSdkResult;
#pragma pack (pop)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace Base {
		/** \brief Generic result of operation */
		template<typename Payload>
		struct Result : GSdkResult {
			std::shared_ptr<Payload> payload;

			Result(
				const int& status,
				const int& domain,
				const std::string& description = "",
				const std::shared_ptr<Payload>& payload = std::shared_ptr<Payload>()) :
				payload(payload) {
				this->status = status;
				this->domain = domain;

				size_t maxLength = GSDK_MAX_RESULT_DESCRIPTION_LENGTH - 1;
				size_t length = description.length();
				if (length > maxLength) {
					length = maxLength;
				}
                std::memcpy(this->description, description.c_str(), length);
				this->description[length] = 0;
			};

			template<typename T> Result(const Result<T>& result) : Result(result.status, result.domain, result.description) {
				// Do nothing more
			};

			operator bool() const {
				return status == 0;
			}

			static Result success(const std::shared_ptr<Payload>& payload = std::shared_ptr<Payload>()) {
				return Result(0, 0, "Success", payload);
			}
		};

		typedef Result<std::vector<unsigned char> > DataResult;
		typedef Result<void> EmptyResult;
	}
}

/** \brief Description of this result */
std::string getString(const GSdkResult& value);

#endif
