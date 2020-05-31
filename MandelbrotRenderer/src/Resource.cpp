#include "Resource.h"

std::string GetEmbeddedTextfile(int resource_id)
{
	HRSRC resource = FindResourceW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(resource_id), MAKEINTRESOURCEW(RCT_TEXTFILE));
	HGLOBAL memory = LoadResource(nullptr, resource);

	if (resource == nullptr)
	{
		int last_err_code = GetLastError();
		throw std::runtime_error("Can't locate text file resource with ID " + std::to_string(resource_id) + " (error code " + std::to_string(last_err_code) + ")");
	}

	size_t resource_size = SizeofResource(nullptr, resource);
	void* resource_pointer = LockResource(memory);

	if (resource_pointer == nullptr)
	{
		int last_err_code = GetLastError();
		throw std::runtime_error("Can't get pointer to loaded text file resource with ID " + std::to_string(resource_id) + " (error code " + std::to_string(last_err_code) + ")");
	}
	else
	{
		std::string result = std::string(reinterpret_cast<char*>(resource_pointer), resource_size);
		return result;
	}
}
