#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <bitset>

//===========================================================================
namespace Pathos
{

//===========================================================================
struct Dir
{
	//=======================================================================
	enum Type : uint8_t
	{
		North = 0,
		East = 1,
		South = 2,
		West = 3
	};

	//=======================================================================
	uint8_t Value;

	//=======================================================================
	constexpr Dir() 
		: Value(North) 
	{}

	//=======================================================================
	constexpr Dir(const Dir& a_Dir) 
		: Value(a_Dir.Value) 
	{}

	//=======================================================================
	constexpr Dir(const Type a_Value) 
		: Value(a_Value) 
	{}

	//=======================================================================
	constexpr Dir& operator=(const Dir& a_Dir)
	{
		Value = a_Dir.Value;
		return *this;
	}

	//=======================================================================
	constexpr Dir& operator=(const Type a_Value)
	{
		Value = a_Value;
		return *this;
	}

	//=======================================================================
	constexpr bool operator==(const Dir& a_Dir) const
	{
		return Value == a_Dir.Value;
	}

	//=======================================================================
	constexpr bool operator!=(const Dir& a_Dir) const
	{
		return Value != a_Dir.Value;
	}

	//=======================================================================
	constexpr operator Type() const
	{
		return static_cast<Type>(Value);
	}

	constexpr Dir GetOpposite() const
	{
		return static_cast<Type>((Value + 2) % 4);
	}

	constexpr Dir GetLeft() const
	{
		return static_cast<Type>((Value + 3) % 4);
	}

	constexpr Dir GetRight() const
	{
		return static_cast<Type>((Value + 1) % 4);
	}
};

//===========================================================================
using EDir = Dir::Type;

//===========================================================================
struct Tile
{
	//=======================================================================
	enum Type : uint8_t
	{
		Closed = 0b00000000,
		EnterNorth = 0b00000001,
		ExitNorth = 0b00000010,
		EnterEast = 0b00000100,
		ExitEast = 0b00001000,
		EnterSouth = 0b00010000,
		ExitSouth = 0b00100000,
		EnterWest = 0b01000000,
		ExitWest = 0b10000000,
		OpenNorth = EnterNorth | ExitNorth,
		OpenEast = EnterEast | ExitEast,
		OpenSouth = EnterSouth | ExitSouth,
		OpenWest = EnterWest | ExitWest,
		AllEntry = EnterNorth | EnterEast | EnterSouth | EnterWest,
		AllExit = ExitNorth | ExitEast | ExitSouth | ExitWest,
		NorthDrop = OpenEast | OpenSouth | OpenWest | ExitNorth,
		EastDrop = OpenNorth | OpenSouth | OpenWest | ExitEast,
		SouthDrop = OpenNorth | OpenEast | OpenWest | ExitSouth,
		WestDrop = OpenNorth | OpenEast | OpenSouth | ExitWest,
		NorthRamp = AllExit | EnterNorth,
		EastRamp = AllExit | EnterEast,
		SouthRamp = AllExit | EnterSouth,
		WestRamp = AllExit | EnterWest,
		NorthFence = OpenEast | OpenSouth | OpenWest,
		EastFence = OpenNorth | OpenSouth | OpenWest,
		SouthFence = OpenNorth | OpenEast | OpenWest,
		WestFence = OpenNorth | OpenEast | OpenSouth,
		Open = AllEntry | AllExit,
	};

	//=======================================================================
	uint8_t Value;

	//=======================================================================
	constexpr Tile() 
		: Value(Open) 
	{}


	//=======================================================================
	constexpr Tile(const Tile& a_Tile) 
		: Value(a_Tile.Value)
	{}

	//=======================================================================
	constexpr Tile(const Type a_Value) 
		: Value(a_Value) 
	{}

	
	//=======================================================================
	constexpr Tile& operator=(const Tile& a_Tile) 
	{
		Value = a_Tile.Value;
		return *this;
	}

	//=======================================================================
	constexpr Tile& operator=(const Type a_Value)
	{
		Value = a_Value;
		return *this;
	}

	//=======================================================================
	constexpr bool operator==(const Dir& a_Dir) const
	{
		return Value == a_Dir.Value;
	}

	//=======================================================================
	constexpr bool operator!=(const Dir& a_Dir) const
	{
		return Value != a_Dir.Value;
	}

	//=======================================================================
	constexpr operator Type() const
	{
		return static_cast<Type>(Value);
	}

	//=======================================================================
	constexpr bool CanEnter(const Dir a_Dir) const
	{
		return Value & (1u << (static_cast<uint8_t>(a_Dir) * 2 + 0));
	}

	//=======================================================================
	constexpr bool CanExit(const Dir a_Dir) const
	{
		return Value & (1u << (static_cast<uint8_t>(a_Dir) * 2 + 1));
	}

	//=======================================================================
	// Can we transit from this tile to the other tile in the given direction?
	constexpr bool CanTransit(const Dir a_Dir, const Tile a_Other) const
	{
		return CanExit(a_Dir) && a_Other.CanEnter(a_Dir.GetOpposite());
	}

	constexpr void SetEnter(const Dir a_Dir, const bool a_Enter)
	{
		if (a_Enter)
		{
			Value |= (1u << (static_cast<uint8_t>(a_Dir) * 2 + 0));
		}
		else
		{
			Value &= ~(1u << (static_cast<uint8_t>(a_Dir) * 2 + 0));
		}
	}

	constexpr void SetExit(const Dir a_Dir, const bool a_Exit)
	{
		if (a_Exit)
		{
			Value |= (1u << (static_cast<uint8_t>(a_Dir) * 2 + 1));
		}
		else
		{
			Value &= ~(1u << (static_cast<uint8_t>(a_Dir) * 2 + 1));
		}
	}
};

//===========================================================================
using ETile = Tile::Type;

//===========================================================================
struct Border
{
	//=======================================================================
	enum Type : uint8_t
	{
		N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15,
		E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15,
		S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15,
		W0, W1, W2, W3, W4, W5, W6, W7, W8, W9, W10, W11, W12, W13, W14, W15
	};

	//=======================================================================
	Type Value;

	//=======================================================================
	constexpr Border()
		: Value(N0)
	{}

	//=======================================================================
	constexpr Border(const Border& a_Border)
		: Value(a_Border.Value)
	{}

	//=======================================================================
	constexpr Border(const Type a_Value)
		: Value(a_Value)
	{}

	//=======================================================================
	constexpr Border& operator=(const Border& a_Border)
	{
		Value = a_Border.Value;
		return *this;
	}

	//=======================================================================
	constexpr Border& operator=(const Type a_Value)
	{
		Value = a_Value;
		return *this;
	}

	//=======================================================================
	constexpr bool operator==(const Border& a_Border) const
	{
		return Value == a_Border.Value;
	}

	//=======================================================================
	constexpr bool operator!=(const Border& a_Border) const
	{
		return Value != a_Border.Value;
	}

	//=======================================================================
	constexpr operator Type() const
	{
		return static_cast<Type>(Value);
	}

	constexpr Dir GetDir() const
	{
		return static_cast<EDir>(static_cast<uint8_t>(Value) >> 4);
	}
};

//===========================================================================
using EBorder = Border::Type;

//===========================================================================
union Coord
{
	//=======================================================================
	uint8_t i = 0;

	//=======================================================================
	struct
	{
		uint8_t x : 4;
		uint8_t y : 4;
	};

	constexpr Coord() 
		: i(0) 
	{}

	//=======================================================================
	constexpr Coord(const Coord& a_Coord) 
		: i(a_Coord.i) 
	{}

	//=======================================================================
	constexpr Coord(const uint8_t a_i) 
		: i(a_i)
	{}

	//=======================================================================
	constexpr Coord(const uint8_t a_x, const uint8_t a_y) 
		: x(a_x), y(a_y) 
	{}

	constexpr Coord(const Border a_Border)
	{
		constexpr Coord Borders[] = 
		{
			{ 0x0, 0x0 }, { 0x1, 0x0 }, { 0x2, 0x0 }, { 0x3, 0x0 }, { 0x4, 0x0 }, { 0x5, 0x0 }, { 0x6, 0x0 }, { 0x7, 0x0 }, { 0x8, 0x0 }, { 0x9, 0x0 }, { 0xA, 0x0 }, { 0xB, 0x0 }, { 0xC, 0x0 }, { 0xD, 0x0 }, { 0xE, 0x0 }, { 0xF, 0x0 }, 
			{ 0xF, 0x0 }, { 0xF, 0x1 }, { 0xF, 0x2 }, { 0xF, 0x3 }, { 0xF, 0x4 }, { 0xF, 0x5 }, { 0xF, 0x6 }, { 0xF, 0x7 }, { 0xF, 0x8 }, { 0xF, 0x9 }, { 0xF, 0xA }, { 0xF, 0xB }, { 0xF, 0xC }, { 0xF, 0xD }, { 0xF, 0xE }, { 0xF, 0xF },
			{ 0xF, 0xF }, { 0xE, 0xF }, { 0xD, 0xF }, { 0xC, 0xF }, { 0xB, 0xF }, { 0xA, 0xF }, { 0x9, 0xF }, { 0x8, 0xF }, { 0x7, 0xF }, { 0x6, 0xF }, { 0x5, 0xF }, { 0x4, 0xF }, { 0x3, 0xF }, { 0x2, 0xF }, { 0x1, 0xF }, { 0x0, 0xF },
			{ 0x0, 0xF }, { 0x0, 0xE }, { 0x0, 0xD }, { 0x0, 0xC }, { 0x0, 0xB }, { 0x0, 0xA }, { 0x0, 0x9 }, { 0x0, 0x8 }, { 0x0, 0x7 }, { 0x0, 0x6 }, { 0x0, 0x5 }, { 0x0, 0x4 }, { 0x0, 0x3 }, { 0x0, 0x2 }, { 0x0, 0x1 }, { 0x0, 0x0 }
		};

		i = Borders[static_cast<uint8_t>(a_Border)].i;
	}

	//=======================================================================
	constexpr Coord& operator=(const Coord& a_Coord)
	{
		i = a_Coord.i;
		return *this;
	}

	//=======================================================================
	constexpr Coord& operator=(const uint8_t a_i)
	{
		i = a_i;
		return *this;
	}

	//=======================================================================
	constexpr bool operator==(const Coord& a_Coord) const
	{
		return i == a_Coord.i;
	}

	//=======================================================================
	constexpr bool operator!=(const Coord& a_Coord) const
	{
		return i != a_Coord.i;
	}

	//=======================================================================
	constexpr bool operator==(const uint8_t a_i) const
	{
		return i == a_i;
	}

	//=======================================================================
	constexpr bool operator!=(const uint8_t a_i) const
	{
		return i != a_i;
	}

	//=======================================================================
	constexpr operator uint8_t() const
	{
		return i;
	}

	//=======================================================================
	constexpr Coord GetNeighbor(const Dir a_Dir) const
	{
		switch (a_Dir)
		{
		case Dir::North:
		return Coord(x, y - 1);
		case Dir::East:
		return Coord(x + 1, y);
		case Dir::South:
		return Coord(x, y + 1);
		case Dir::West:
		return Coord(x - 1, y);
		default:
		return *this; // Should never happen
		}
	}

	//=======================================================================
	constexpr Coord GetNorth() const 
	{ 
		return Coord(x, y - 1); 
	}

	//=======================================================================
	constexpr Coord GetEast() const 
	{ 
		return Coord(x + 1, y); 
	}

	//=======================================================================
	constexpr Coord GetSouth() const 
	{ 
		return Coord(x, y + 1); 
	}

	//=======================================================================
	constexpr Coord GetWest() const 
	{ 
		return Coord(x - 1, y); 
	}

	//=======================================================================
	constexpr bool IsNorthBorder() const
	{
		return y == 0;
	}

	//=======================================================================
	constexpr bool IsEastBorder() const
	{
		return x == 15;
	}

	//=======================================================================
	constexpr bool IsSouthBorder() const
	{
		return y == 15;
	}

	//=======================================================================
	constexpr bool IsWestBorder() const
	{
		return x == 0;
	}

	//=======================================================================
	constexpr bool IsBorder(const EDir a_Dir) const
	{
		switch (a_Dir)
		{
		case EDir::North:
			return IsNorthBorder();
		case EDir::East:
			return IsEastBorder();
		case EDir::South:
			return IsSouthBorder();
		case EDir::West:
			return IsWestBorder();
		default:
			return false;
		}
	}

	//=======================================================================
	constexpr bool IsBorder() const
	{
		return IsNorthBorder() || IsEastBorder() || IsSouthBorder() || IsWestBorder();
	}

	//=======================================================================
	constexpr bool IsCorner() const
	{
		return (IsNorthBorder() || IsSouthBorder()) && (IsEastBorder() || IsWestBorder());
	}
};

//===========================================================================
struct Portal
{
	uint8_t Start;
	uint8_t Count;
	uint64_t Exits;
};

//===========================================================================
struct Chunk
{
	//=======================================================================
	Tile Tiles[16 * 16];

	//=======================================================================
	constexpr Tile& operator[](const Coord a_Coord)
	{
		return Tiles[a_Coord.i];
	}

	//=======================================================================
	constexpr Tile operator[](const Coord a_Coord) const
	{
		return Tiles[a_Coord.i];
	}

	//=======================================================================
	constexpr bool CanEnter(const Coord a_Coord, const Dir a_Dir) const
	{
		return Tiles[a_Coord.i].CanEnter(a_Dir);
	}

	//=======================================================================
	constexpr bool CanEnter(const Border a_Border) const
	{
		return CanEnter(Coord(a_Border), a_Border.GetDir());
	}

	//=======================================================================
	constexpr bool CanExit(const Coord a_Coord, const Dir a_Dir) const
	{
		return Tiles[a_Coord.i].CanExit(a_Dir);
	}

	//=======================================================================
	constexpr bool CanExit(const Border a_Border) const
	{
		return CanExit(Coord(a_Border), a_Border.GetDir());
	}

	//=======================================================================
	constexpr bool CanMove(const Coord a_Coord, const Dir a_Dir, Coord& o_Moved) const
	{
		switch (a_Dir)
		{
		case EDir::North:
		{
			return !a_Coord.IsNorthBorder() && CanExit(a_Coord, a_Dir) && CanEnter(o_Moved = a_Coord.GetNorth(), a_Dir.GetOpposite());
		}
		case EDir::East:
		{
			return !a_Coord.IsEastBorder() && CanExit(a_Coord, a_Dir) && CanEnter(o_Moved = a_Coord.GetEast(), a_Dir.GetOpposite());
		}
		case EDir::South:
		{
			return !a_Coord.IsSouthBorder() && CanExit(a_Coord, a_Dir) && CanEnter(o_Moved = a_Coord.GetSouth(), a_Dir.GetOpposite());
		}
		case EDir::West:
		{
			return !a_Coord.IsWestBorder() && CanExit(a_Coord, a_Dir) && CanEnter(o_Moved = a_Coord.GetWest(), a_Dir.GetOpposite());
		}
		default:
		return false;
		}
	}

	//=======================================================================
	std::vector<Portal> GetPortals() const
	{
		std::array<uint64_t, 64> Portals;
		uint8_t PortalCount = 0;

		// This represents the exits from each coord given an entry direction.
		// Index = EntryCoord.i * EntryDir;
		std::array<uint64_t, 16 * 16 * 4> Exits{};
		std::bitset<16 * 16 * 4> Visited{};

		std::array<uint64_t, 16 * 16> Unvisited{};
		uint8_t UnvisitedCount = 0;

		// Iterate through all border points.
		for (uint8_t i = 0; i < 64; ++i)
		{
			const Border B = static_cast<EBorder>(i);

			if (!CanEnter(B))
			{
				Portals[PortalCount++] = 0;
				continue; // Skip borders that cannot be entered
			}

			const Dir EntryDir = B.GetDir();

			// Start at this border tile.
			const Coord EntryCoord(B);

			// What are the neighbouring tiles that can be exited to? (i.e. what are the portals from this tile)
			const EDir LeftDir = EntryDir.GetLeft();
			if (Coord MovedLeft; CanMove(EntryCoord, LeftDir, MovedLeft))
			{
				const size_t ExitIndex = static_cast<size_t>(MovedLeft.i) * 4 + static_cast<size_t>(LeftDir);

				// Check if we have entered this very tile from this direction before?
				if (!Visited.test(ExitIndex))
				{
					// Mark this tile as visited from this direction.
					Visited.set(ExitIndex);

					// Add to unvisited list.
					Unvisited[UnvisitedCount++] = ExitIndex;
				}
				else
				{
					// We have already visited this tile from this direction, so we can skip it.
				}
			}

			const Dir ExitBack = EntryDir.GetOpposite();
			const Dir ExitRight = EntryDir.GetRight();
		}

		return std::vector<Portal>(Portals.begin(), Portals.begin() + PortalCount);
	}
};

using PortalVector = uint64_t;
}