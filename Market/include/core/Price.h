/**
 * @file Price.h
 * @author Arnav Deshpande
 * @brief Defines the Price value type used across the MicroEx engine.
 *
 * Price represents a monetary value with advisory semantic validity.
 * Arithmetic operations always execute, while validity propagates
 * according to domain rules (negative or NaN values are invalid).
 */

#pragma once

#include <core/Core.h>
#include <cmath>

namespace MicroEx::ValueTypes
{

	/**
	 * @class Price
	 * @brief Represents a price value with advisory validity.
	 *
	 * Price is a lightweight value type that stores a double precision
	 * numeric value along with a semantic validity flag.
	 *
	 * Validity is informational: arithmetic always executes even on
	 * invalid values, but invalidity propagates to results.
	 */
	class MICROEX_API Price final
	{
	public:
		/**
		 * @brief Constructs a valid price with value 0.0.
		 */
		constexpr Price() noexcept
			: m_Value(0.0), m_IsValid(true)
		{
		}

		/**
		 * @brief Constructs a price from a double value.
		 * @param value Numeric price value.
		 */
		explicit Price(double value) noexcept
			: m_Value(value), m_IsValid(true)
		{
			m_Validate();
		}

		~Price() noexcept = default;

		/**
		 * @brief Returns the stored numeric value.
		 */
		constexpr double Get() const noexcept
		{
			return m_Value;
		}

		/**
		 * @brief Indicates whether the price is semantically valid.
		 */
		constexpr bool IsValid() const noexcept
		{
			return m_IsValid;
		}

		/**
		 * @brief Assigns a new numeric value to the price.
		 */
		void Set(double value) noexcept
		{
			m_Value = value;
			m_Validate();
		}

		/// Compound arithmetic with double
		Price& operator+=(double value) noexcept { return *this += Price(value); }
		Price& operator-=(double value) noexcept { return *this -= Price(value); }
		Price& operator*=(double value) noexcept { return *this *= Price(value); }

		Price& operator/=(double value) noexcept
		{
			if (value == 0.0)
			{
				m_Value = std::nan("");
				m_IsValid = false;
			}
			else
			{
				m_Value /= value;
				m_Validate();
			}
			return *this;
		}

		/// Compound arithmetic with Price
		Price& operator+=(const Price& other) noexcept
		{
			m_Value += other.m_Value;
			m_IsValid = m_IsValid && other.m_IsValid;
			m_Validate();
			return *this;
		}

		Price& operator-=(const Price& other) noexcept
		{
			m_Value -= other.m_Value;
			m_IsValid = m_IsValid && other.m_IsValid;
			m_Validate();
			return *this;
		}

		Price& operator*=(const Price& other) noexcept
		{
			m_Value *= other.m_Value;
			m_IsValid = m_IsValid && other.m_IsValid;
			m_Validate();
			return *this;
		}

		Price& operator/=(const Price& other) noexcept
		{
			if (other.m_Value == 0.0)
			{
				m_Value = std::nan("");
				m_IsValid = false;
			}
			else
			{
				m_Value /= other.m_Value;
				m_IsValid = m_IsValid && other.m_IsValid;
				m_Validate();
			}
			return *this;
		}

		/**
		 * @brief Converts price to its numeric value.
		 */
		constexpr explicit operator double() const noexcept
		{
			return m_Value;
		}

		/**
		 * @brief Boolean conversion reflects semantic validity.
		 */
		constexpr explicit operator bool() const noexcept
		{
			return m_IsValid;
		}

		/**
		 * @brief Creates an invalid price with value 0.0.
		 */
		static Price Invalid() noexcept
		{
			Price p;
			p.m_IsValid = false;
			return p;
		}

		/**
		 * @brief Creates an invalid NaN price.
		 */
		static Price NaN() noexcept
		{
			Price p;
			p.m_Value = std::nan("");
			p.m_IsValid = false;
			return p;
		}

	private:
		void m_Validate() noexcept
		{
			if (m_Value < 0.0 || std::isnan(m_Value))
				m_IsValid = false;
		}

	private:
		double m_Value;
		bool m_IsValid;
	};

	/// Non-member arithmetic operators
	inline Price operator+(const Price& lhs, const Price& rhs) noexcept
	{
		Price p(lhs.Get() + rhs.Get());
		p = (lhs.IsValid() && rhs.IsValid()) ? p : Price::Invalid();
		return p;
	}

	inline Price operator-(const Price& lhs, const Price& rhs) noexcept
	{
		Price p(lhs.Get() - rhs.Get());
		p = (lhs.IsValid() && rhs.IsValid()) ? p : Price::Invalid();
		return p;
	}

	inline Price operator*(const Price& lhs, const Price& rhs) noexcept
	{
		Price p(lhs.Get() * rhs.Get());
		p = (lhs.IsValid() && rhs.IsValid()) ? p : Price::Invalid();
		return p;
	}

	inline Price operator/(const Price& lhs, const Price& rhs) noexcept
	{
		if (rhs.Get() == 0.0)
			return Price::NaN();

		Price p(lhs.Get() / rhs.Get());
		p = (lhs.IsValid() && rhs.IsValid()) ? p : Price::Invalid();
		return p;
	}

	inline bool operator==(const Price& lhs, const Price& rhs) noexcept
	{
		return lhs.Get() == rhs.Get();
	}

	inline bool operator!=(const Price& lhs, const Price& rhs) noexcept
	{
		return lhs.Get() != rhs.Get();
	}

	inline const Price ZeroPrice = Price(0.0);
	inline const Price InvalidPrice = Price::Invalid();

} //namespace MicroEx::ValueTypes

namespace MicroEx
{
	using price_t = ::MicroEx::ValueTypes::Price;
} //namespace MicroEx
