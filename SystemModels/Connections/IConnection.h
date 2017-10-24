#pragma once
#ifndef ICONNECTION_H_
#define ICONNECTION_H_

#include "../TimeSpan.h"

namespace SystemModels
{
	//Interrelationships between several systems
	class IConnection
	{
	public:
		IConnection() {}
		virtual ~IConnection() {};
		virtual void Interact(TimeSpan* timeSpan) = 0;
	};
}

#endif /*ICONNECTION_H_*/
