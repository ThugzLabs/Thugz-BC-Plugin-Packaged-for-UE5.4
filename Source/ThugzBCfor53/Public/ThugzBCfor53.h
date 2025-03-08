// Copyright (c) 2025 Thugz Labs
// All Rights Reserved.
//
// This software is provided "as-is," without any express or implied warranty.
// In no event shall the authors or copyright holders be held liable for any damages 
// arising from the use of this software.
//
// You are permitted to use, modify, and distribute this software under the terms 
// of the license specified in the LICENSE file included with this repository.
//
// Publisher: Thugz Labs
// Year of Publication: 2025


#pragma once

#include "Modules/ModuleManager.h"

class FThugzBCfor53Module : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
