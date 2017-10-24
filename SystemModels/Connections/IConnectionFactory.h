#pragma once
#ifndef ICONNECTIONFACTORY_H_
#define ICONNECTIONFACTORY_H_

#include <boost/smart_ptr/shared_ptr.hpp>

namespace SystemModels
{
	template <class ISystem1, class ISystem2, class IConnectionType>
	class IConnectionFactory
	{
	public:
		IConnectionFactory() {}
		virtual ~IConnectionFactory() {};

		virtual std::shared_ptr<IConnectionType> Create(boost::shared_ptr<ISystem1> system1, boost::shared_ptr<ISystem2> system2) = 0;
	};
}

#endif /*ICONNECTIONFACTORY_H_*/
