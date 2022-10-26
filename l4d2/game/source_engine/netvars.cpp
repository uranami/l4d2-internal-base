#include "../../framework.h"
#include "../game.h"
#include "netvars.h"

namespace Game
{

#define DUMP_NETVARS

#ifdef DUMP_NETVARS
#define IF_DUMPING(...) __VA_ARGS__
#else
#define IF_DUMPING(...)
#endif

	IF_DUMPING(static FILE* s_fp;)

		const char* prop_type[7] =
	{
		"int",
		"float",
		"vec3",
		"vec2",
		"char *",
		"array",
		"table"
	};

	NetVar_t::NetVar_t()
	{
		IF_DUMPING(fopen_s(&s_fp, "netvar_dump.txt", "w");)
			for (auto clazz = Interfaces->Client->GetAllClasses(); clazz; clazz = clazz->m_pNext)
				if (clazz->m_pRecvTable)
					DUMP_NETVAR(clazz->m_pNetworkName, clazz->m_pRecvTable, 0);
		IF_DUMPING(fclose(s_fp);)
	}

	auto NetVar_t::DUMP_NETVAR(const char* base_class, RecvTable* table, const std::uint16_t offset) -> void
	{
		for (auto i = 0; i < table->m_nProps; ++i)
		{
			const auto prop_ptr = &table->m_pProps[i];

			if (!prop_ptr || isdigit(prop_ptr->m_pVarName[0]))
				continue;

			if (strcmp(prop_ptr->m_pVarName, "baseclass") == 0)
				continue;

			if (prop_ptr->m_pDataTable != nullptr)
				DUMP_NETVAR(base_class, prop_ptr->m_pDataTable, std::uint16_t(offset + prop_ptr->m_Offset));

			char hash_name[256];

			strcpy_s(hash_name, base_class);
			strcat_s(hash_name, "->");
			strcat_s(hash_name, prop_ptr->m_pVarName);

			const auto hash = fnv::hash_runtime(hash_name);
			const auto total_offset = std::uint16_t(offset + prop_ptr->m_Offset);

			IF_DUMPING(fprintf(s_fp, " [%s]\t%s->%s = 0x%04X\n", prop_type[prop_ptr->m_RecvType], base_class, prop_ptr->m_pVarName, total_offset);)

				m_props[hash] =
			{
				prop_ptr,
				total_offset
			};
		}
	}
}
