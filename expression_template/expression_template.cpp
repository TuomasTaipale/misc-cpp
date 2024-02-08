/* g++ -std=c++17 expression_template.cpp -o expression_template */

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace
{
	template <typename A>
	struct Expression
	{
		auto value() const
		{
			return static_cast<const A&>(*this).value();
		}

	};

	template <typename A, typename B>
	struct ContainerSum: public Expression<ContainerSum<A, B>>
	{
		const A& op1;
		const B& op2;

		ContainerSum(const A& op1, const B& op2):
			op1 { op1 },
			op2 { op2 }
		{
		}

		auto value() const
		{
			return op1.value() + op2.value();
		}
	};

	template <typename A, typename B>
	ContainerSum<A, B> operator+(const Expression<A>& op1, const Expression<B>& op2)
	{
		return ContainerSum<A, B> { static_cast<const A&>(op1),
					    static_cast<const B&>(op2) };
	}

	template <typename A, typename B>
	std::ostream& operator<<(std::ostream& os, const ContainerSum<A, B>& c)
	{
		return os << "[" << std::addressof(c) << "]: " << c.value();
	}

	template <typename A>
	class Container: public Expression<Container<A>>
	{
	public:
		Container():
			val { 0U }
		{
			std::cout << "[" << this << "] def ctor called" << std::endl;
		}

		Container(A val):
			val { val }
		{
			std::cout << "[" << this << "] ctor called" << std::endl;
		}

		Container(const Container& c)
		{
			std::cout << "[" << this << "] cp ctor called" << std::endl;
			val = c.val;
		}

		Container& operator=(const Container& c)
		{
			std::cout << "[" << this << "] = ctor called" << std::endl;
			val = c.val;

			return *this;
		}

		template <typename B>
		Container(const Expression<B>& e)
		{
			std::cout << "[" << this << "] expr cp ctor called" << std::endl;
			val = e.value();
		}

		template <typename B>
		Container& operator=(const Expression<B>& e)
		{
			std::cout << "[" << this << "] expr = ctor called" << std::endl;
			val = e.value();

			return *this;
		}

		A value() const
		{
			return val;
		}

	private:
		A val;
	};

	template <typename A>
	std::ostream& operator<<(std::ostream& os, const Container<A>& c)
	{
		return os << "[" << std::addressof(c) << "]: " << c.value();
	}
}

int main()
{
	Container<uint32_t> c1 { 111U }, c2 { 222U }, c4, c7;
	Container c3 = c2;

	c4 = c3;
	std::cout << "Container c1 value: " << c1 << std::endl;
	std::cout << "Container c2 value: " << c2 << std::endl;
	std::cout << "Container c3 value: " << c3 << std::endl;
	std::cout << "Container c4 value: " << c4 << std::endl;
	std::cout << "No evaluation yet!" << std::endl;

	auto c5 = c1 + c2 + c3 + c4;

	std::cout << "Now they are!" << std::endl;

	Container<uint32_t> c6 = c1 + c2 + c3 + c4;

	c7 = c1 + c2 + c3 + c4;
	std::cout << "sum1: " << c1 + c2 + c3 + c4 << std::endl;
	std::cout << "sum2: " << c6 << std::endl;
	std::cout << "sum3: " << c7 << std::endl;

	return EXIT_SUCCESS;
}

