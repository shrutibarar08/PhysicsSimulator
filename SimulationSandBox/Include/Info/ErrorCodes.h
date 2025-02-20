#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include <string>
#include <unordered_map>

// Define error codes
enum class ErrorCode: uint8_t
{
#define X(code, message) code,
#include "ErrorCodes.def"
#undef X
};

// Function to map error codes to messages
inline const std::wstring& GetSimulationErrorMessage(ErrorCode code) {
    static const std::unordered_map<ErrorCode, std::wstring> errorMessages = {
#define X(code, message) { ErrorCode::code, L##message },
#include "ErrorCodes.def"
#undef X
    };

    static const std::wstring unknownMessage = L"Unrecognized error code.";
    auto it = errorMessages.find(code);
    return (it != errorMessages.end()) ? it->second : unknownMessage;
}

#endif // ERROR_CODES_H
