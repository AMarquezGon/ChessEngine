#pragma once
#include <cstdint>

namespace constants
{
	constexpr std::uint64_t RANK_1 = 0xffULL;
	constexpr std::uint64_t RANK_2 = 0xff00ULL;
	constexpr std::uint64_t RANK_3 = 0xff0000ULL;
	constexpr std::uint64_t RANK_4 = 0xff000000ULL;
	constexpr std::uint64_t RANK_5 = 0xff00000000ULL;
	constexpr std::uint64_t RANK_6 = 0xff0000000000ULL;
	constexpr std::uint64_t RANK_7 = 0xff000000000000ULL;
	constexpr std::uint64_t RANK_8 = 0xff00000000000000ULL;

	constexpr std::uint64_t FILE_A = 0x0101010101010101ULL;
	constexpr std::uint64_t FILE_B = 0x0202020202020202ULL;
	constexpr std::uint64_t FILE_C = 0x0404040404040404ULL;
	constexpr std::uint64_t FILE_D = 0x0808080808080808ULL;
	constexpr std::uint64_t FILE_E = 0x1010101010101010ULL;
	constexpr std::uint64_t FILE_F = 0x2020202020202020ULL;
	constexpr std::uint64_t FILE_G = 0x4040404040404040ULL;
	constexpr std::uint64_t FILE_H = 0x8080808080808080ULL;

}