#pragma once

#include "../resources/resource.h"

#include <Windows.h>
#include <WinBase.h>
#include <atlbase.h>

#include <string>
#include <stdexcept>

std::string GetEmbeddedTextfile(int resource_id);