#pragma once

#include <unordered_map>


namespace Game
{
	class NetVar_t
	{
	private:
		struct stored_data
		{
			RecvProp* prop_ptr;
			std::uint16_t class_relative_offset;
		};

	public:
		// Return a const instance, others shouldnt modify this.
		static auto Get() -> const NetVar_t&
		{
			static NetVar_t instance;
			return instance;
		}

		auto GetOffset(const fnv::hash hash) const -> std::uint16_t
		{
			return m_props.at(hash).class_relative_offset;
		}

		auto GetProP(const fnv::hash hash) const -> RecvProp*
		{
			return m_props.at(hash).prop_ptr;
		}

		// Prevent instruction cache pollution caused by automatic
		// inlining of `get` and get_offset every netvar usage when
		// there are a lot of netvars
		__declspec(noinline) static auto GetOffsetByHASH(const fnv::hash hash) -> std::uint16_t
		{
			return Get().GetOffset(hash);
		}

		template<fnv::hash Hash>
		static auto ToAddress(void* ptr) -> std::uintptr_t
		{
			static auto offset = std::uint16_t(0);
			if (!offset)
				offset = GetOffsetByHASH(Hash);
			return std::uintptr_t(ptr) + offset;
		}

	private:
		NetVar_t();
		auto DUMP_NETVAR(const char* base_class, RecvTable* table, std::uint16_t offset) -> void;

	private:
		std::unordered_map<fnv::hash, stored_data> m_props;
	};
}


#define PNETVAR_OFFSET(funcname, class_name, var_name, offset, ...) \
auto funcname() -> std::add_pointer_t<__VA_ARGS__> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = Game::NetVar_t::ToAddress<hash>(this); \
	return reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(addr); \
}

#define PNETVAR(funcname, class_name, var_name, ...) \
	PNETVAR_OFFSET(funcname, class_name, var_name, 0, __VA_ARGS__)

#define NETVAR_OFFSET(funcname, class_name, var_name, offset, ...) \
auto funcname() -> std::add_lvalue_reference_t<__VA_ARGS__> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = Game::NetVar_t::ToAddress<hash>(this + offset); \
	return *reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(addr); \
}
#define NETVAR(funcname, class_name, var_name, ...) \
	NETVAR_OFFSET(funcname, class_name, var_name, 0, __VA_ARGS__)

#define NETPROP(funcname, class_name, var_name) \
static auto funcname() ->  RecvProp* \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	static auto prop_ptr = Game::NetVar_t::Get().GetProP(hash); \
	return prop_ptr; \
}