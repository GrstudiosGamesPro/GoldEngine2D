#pragma once
#include "../AComponent.h"

class CAudio : public AComponent
{
public:
	void start  () override;
	void draw (Entity* owner) override;
};