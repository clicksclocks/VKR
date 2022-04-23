#pragma once
#include <Horyzen/EntityComponentSystem/Component.h>

#include <iostream>

struct TestComponent : public Horyzen::ECS::Component<TestComponent>
{
	~TestComponent()
	{
		std::cout << "I am the destructor!\n";
	}
	float x, y;
};

struct TestComponent1 : public Horyzen::ECS::Component<TestComponent1>
{
	float x, y;
};
