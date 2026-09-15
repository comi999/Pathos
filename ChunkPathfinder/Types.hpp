#pragma once
#include <cstdint>
#include <array>
#include <iostream>
#include <vector>
#include <bitset>
#include <queue>
#include <map>

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
	constexpr bool operator==(const Dir a_Dir) const
	{
		return Value == a_Dir.Value;
	}

	//=======================================================================
	constexpr bool operator!=(const Dir a_Dir) const
	{
		return Value != a_Dir.Value;
	}

	//=======================================================================
	constexpr std::strong_ordering operator<=>(const Dir a_Dir) const
	{
		return Value <=> a_Dir.Value;
	}

	//=======================================================================
	constexpr std::strong_ordering operator<=>(const Type a_Dir) const
	{
		return Value <=> a_Dir;
	}

	//=======================================================================
	constexpr Dir& operator++()
	{
		Value = static_cast<Type>((Value + 1) % 4);
		return *this;
	}

	//=======================================================================
	constexpr Dir operator++(int)
	{
		Dir Temp = *this;
		++(*this);
		return Temp;
	}

	//=======================================================================
	constexpr Dir& operator--()
	{
		Value = static_cast<Type>((Value + 3) % 4);
		return *this;
	}

	//=======================================================================
	constexpr Dir operator--(int)
	{
		Dir Temp = *this;
		--(*this);
		return Temp;
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
	
	constexpr Dir GetNext() const
	{
		return  static_cast<Type>((Value + 1) % 4);
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
	constexpr bool operator==(const Dir a_Dir) const
	{
		return Value == a_Dir.Value;
	}

	//=======================================================================
	constexpr bool operator==(const Type a_Dir) const
	{
		return Value == a_Dir;
	}

	
	//=======================================================================
	constexpr bool operator!=(const Dir a_Dir) const
	{
		return Value != a_Dir.Value;
	}

	//=======================================================================
	constexpr bool operator!=(const Type a_Dir) const
	{
		return Value != a_Dir;
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
		W0, W1, W2, W3, W4, W5, W6, W7, W8, W9, W10, W11, W12, W13, W14, W15,

		First = N0,
		Last = W15,
		Count = 64
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
	constexpr Border(const uint8_t a_Value)
		: Value(static_cast<Type>(a_Value))
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
	constexpr Border& operator=(const uint8_t a_Value)
	{
		Value = static_cast<Type>(a_Value);
		return *this;
	}

	//=======================================================================
	constexpr bool operator==(const Border a_Border) const
	{
		return Value == a_Border.Value;
	}

	//=======================================================================
	constexpr bool operator==(const Type a_Border) const
	{
		return Value == a_Border;
	}

	//=======================================================================
	constexpr bool operator==(const uint8_t a_Border) const
	{
		return Value == static_cast<Type>(a_Border);
	}

	//=======================================================================
	constexpr bool operator!=(const Border a_Border) const
	{
		return Value != a_Border.Value;
	}

	//=======================================================================
	constexpr bool operator!=(const Type a_Border) const
	{
		return Value != a_Border;
	}

	//=======================================================================
	constexpr bool operator!=(const uint8_t a_Border) const
	{
		return Value != static_cast<Type>(a_Border);
	}

	//=======================================================================
	constexpr std::strong_ordering operator<=>(const Border a_Border) const
	{
		return Value <=> a_Border.Value;
	}

	//=======================================================================
	constexpr std::strong_ordering operator<=>(const Type a_Border) const
	{
		return Value <=> a_Border;
	}

	//=======================================================================
	constexpr std::strong_ordering operator<=>(const uint8_t a_Border) const
	{
		return Value <=> static_cast<Type>(a_Border);
	}

	//=======================================================================
	constexpr Border& operator++()
	{
		Value = static_cast<Type>((static_cast<uint8_t>(Value) + 1) % 64);
		return *this;
	}

	//=======================================================================
	constexpr Border operator++(int)
	{
		Border Temp = *this;
		++(*this);
		return Temp;
	}

	//=======================================================================
	constexpr Border& operator--()
	{
		Value = static_cast<Type>((static_cast<uint8_t>(Value) + 63) % 64);
		return *this;
	}

	//=======================================================================
	constexpr Border operator--(int)
	{
		Border Temp = *this;
		--(*this);
		return Temp;
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
		const uint8_t Idx = static_cast<uint8_t>(a_Border.Value);
		if (Idx < 16)
		{
			x = Idx;
			y = 0;
		}
		else if (Idx < 32)
		{
			x = 15;
			y = Idx - 16;
		}
		else if (Idx < 48)
		{
			x = 15 - (Idx - 32);
			y = 15;
		}
		else
		{
			x = 0;
			y = 15 - (Idx - 48);
		}
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
		std::array<uint64_t, 16 * 16> Portals{};
		uint8_t PortalCount = 0;

		struct PFNode;

		std::map<uint64_t, PFNode> PFNodes;

		struct PFNode
		{
			PFNode* Parents[4] = { nullptr, nullptr, nullptr, nullptr };
		};

		for (uint8_t BIdx = 0; BIdx < Border::Count; ++BIdx)
		{
			const Border B(BIdx);

			if (!CanEnter(B))
			{
				Portals[PortalCount++] = 0;
				continue;
			}

			std::queue<std::pair<uint64_t, PFNode*>> OpenList;

			const Coord StartCoord(B);
			const Dir StartDir(B.GetDir()); // This is relative to border tile.
			const uint64_t StartNode = (static_cast<uint64_t>(StartCoord.i) << 2) | static_cast<uint64_t>(StartDir.Value);

			while (!OpenList.empty())
			{
				// Check what neighbours we can move to from this node.
				const Dir Dir0 = NodeDir.GetNext();
				const Dir Dir1 = Dir0.GetNext();
				const Dir Dir2 = Dir1.GetNext();

				if (Coord Dir0Coord; CanMove(NodeCoord, Dir0, Dir0Coord))
				{
					HandleTop(Dir0Coord, Dir0);
				}

				if (Coord Dir1Coord; CanMove(NodeCoord, Dir1, Dir1Coord))
				{
					HandleTop(Dir1Coord, Dir1);
				}

				if (Coord Dir2Coord; CanMove(NodeCoord, Dir2, Dir2Coord))
				{
					HandleTop(Dir2Coord, Dir2);
				}
			}
		}

		std::vector<Portal> Collapsed;

		// ... Collapse the portals into a vector of Portal structs

		return Collapsed;
	}
};

using PortalVector = uint64_t;
}