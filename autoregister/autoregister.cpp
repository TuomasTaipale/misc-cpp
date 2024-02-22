/* g++ autoregister.cpp -o autoregister */

#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>

namespace
{
	class Registry
	{
	public:
		using Name = std::string;
		using Handler = std::function<void(const Name&)>;

		void registerHandler(const Name& name, const Handler& handler)
		{
			(void)handlers.insert({ name, handler });
		}

		void handle()
		{
			for (const auto& i: handlers)
				i.second(i.first);
		}

		static Registry& getInstance();

	private:
		std::map<Name, Handler> handlers;
	};

	Registry& Registry::getInstance()
	{
		static Registry registry;

		return registry;
	}

#define S_(a, b) a##b
#define S(a, b) S_(a, b)
#define AUTOREGISTER(_name, _handler) \
	namespace \
	{ \
		struct S(Autoregister, __LINE__) \
		{ \
			S(Autoregister, __LINE__)() \
			{ \
				::Registry::getInstance().registerHandler(_name, _handler); \
			} \
 \
			~S(Autoregister, __LINE__)() \
			{ \
			} \
		}; \
 \
		S(Autoregister, __LINE__) S(autoregister, __LINE__); \
	}

	struct HandlerA
	{
		void handle(const Registry::Name& name)
		{
			std::cout << "hello, world, from: " << name << std::endl;
		}
	};

	struct HandlerB
	{
		void handle(const Registry::Name& name)
		{
			std::cout << "hello, world, from: " << name << std::endl;
		}
	};
}

AUTOREGISTER("handle A", [](const Registry::Name& name)
	{
		HandlerA a;

		a.handle(name);
	});

AUTOREGISTER("handle B", [](const Registry::Name& name)
	{
		HandlerB b;

		b.handle(name);
	});

int main()
{
	Registry::getInstance().handle();

	return EXIT_SUCCESS;
}