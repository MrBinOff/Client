#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <utils/string.hpp>

#include "game.hpp"
#include "dvars.hpp"
#include <component/console.hpp>
#include <utils/hook.hpp>

namespace dvars
{
	std::unordered_map<std::int32_t, dvar_info> dvar_map;

	game::dvar_s* con_inputBoxColor = nullptr;
	game::dvar_s* con_inputHintBoxColor = nullptr;
	game::dvar_s* con_outputBarColor = nullptr;
	game::dvar_s* con_outputSliderColor = nullptr;
	game::dvar_s* con_outputWindowColor = nullptr;
	game::dvar_s* con_inputDvarMatchColor = nullptr;
	game::dvar_s* con_inputDvarValueColor = nullptr;
	game::dvar_s* con_inputDvarInactiveValueColor = nullptr;
	game::dvar_s* con_inputCmdMatchColor = nullptr;

	std::string dvar_get_vector_domain(const int components, const game::DvarLimits& domain)
	{
		if (domain.vector.min == -FLT_MAX)
		{
			if (domain.vector.max == FLT_MAX)
			{
				return utils::string::va("Domain is any %iD vector", components);
			}
			else
			{
				return utils::string::va("Domain is any %iD vector with components %g or smaller", components,
					domain.vector.max);
			}
		}
		else if (domain.vector.max == FLT_MAX)
		{
			return utils::string::va("Domain is any %iD vector with components %g or bigger", components,
				domain.vector.min);
		}
		else
		{
			return utils::string::va("Domain is any %iD vector with components from %g to %g", components,
				domain.vector.min, domain.vector.max);
		}
	}

	std::string dvar_get_domain(const game::dvar_type type, const game::DvarLimits& domain)
	{
		std::string str;

		switch (type)
		{
		case game::dvar_type::boolean:
			return "Domain is 0 or 1"s;

		case game::dvar_type::value:
			if (domain.value.min == -FLT_MAX)
			{
				if (domain.value.max == FLT_MAX)
				{
					return "Domain is any number"s;
				}
				else
				{
					return utils::string::va("Domain is any number %g or smaller", domain.value.max);
				}
			}
			else if (domain.value.max == FLT_MAX)
			{
				return utils::string::va("Domain is any number %g or bigger", domain.value.min);
			}
			else
			{
				return utils::string::va("Domain is any number from %g to %g", domain.value.min, domain.value.max);
			}

		case game::dvar_type::vec2:
			return dvar_get_vector_domain(2, domain);

		case game::dvar_type::rgb:
		case game::dvar_type::vec3:
			return dvar_get_vector_domain(3, domain);

		case game::dvar_type::vec4:
			return dvar_get_vector_domain(4, domain);

		case game::dvar_type::integer:
			if (domain.enumeration.stringCount == INT_MIN)
			{
				if (domain.integer.max == INT_MAX)
				{
					return "Domain is any integer"s;
				}
				else
				{
					return utils::string::va("Domain is any integer %i or smaller", domain.integer.max);
				}
			}
			else if (domain.integer.max == INT_MAX)
			{
				return utils::string::va("Domain is any integer %i or bigger", domain.integer.min);
			}
			else
			{
				return utils::string::va("Domain is any integer from %i to %i", domain.integer.min, domain.integer.max);
			}

		case game::dvar_type::color:
			return "Domain is any 4-component color, in RGBA format"s;

		case game::dvar_type::enumeration:
			str = "Domain is one of the following:"s;

			for (auto string_index = 0; string_index < domain.enumeration.stringCount; ++string_index)
			{
				str += utils::string::va("\n  %2i: %s", string_index, domain.enumeration.strings[string_index]);
			}

			return str;

		case game::dvar_type::string:
			return "Domain is any text"s;

		default:
			return utils::string::va("unhandled dvar type '%i'", type);
		}
	}

	game::dvar_s* Dvar_RegisterVec4(const char* dvar_name, const char* description, float x, float y, float z, float w, float min_value, float max_value, std::uint16_t flags) 
	{
		game::DvarValue value{};
		value.vector[0] = x;
		value.vector[1] = y;
		value.vector[2] = z;
		value.vector[3] = w;

		game::DvarLimits domain{};
		domain.vector.max = max_value;
		domain.vector.min = min_value;
		return game::Dvar_RegisterNew(dvar_name, game::DvarType::DVAR_TYPE_FLOAT_4, flags, value, domain);
	}
}