#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <array>
#include <iomanip>

#include "Types.hpp"

union Coord
{
	uint8_t i = 0;
	struct { uint8_t x : 4, y : 4; };
};

static constexpr Tile& GetTile(Chunk& a_Chunk, const Coord a_Coord)
{
	return a_Chunk[a_Coord.i];
}

static constexpr Tile GetTile(const Chunk& a_Chunk, const Coord a_Coord)
{
	return a_Chunk[a_Coord.i];
}



static constexpr EBorder operator+(const EBorder a_Border, const int32_t a_Offset)
{
	return static_cast<EBorder>(static_cast<uint8_t>(a_Border) + a_Offset);
}

static constexpr Coord GetCoord(const EBorder a_Border)
{
	const uint8_t Idx = static_cast<uint8_t>(a_Border);
	Coord C{};
	if (Idx < 16)
	{
		// Top edge left -> right
		C.x = Idx;
		C.y = 0;
	}
	else if (Idx < 32)
	{
		// Right edge top -> bottom (including top corner at E0)
		C.x = 15;
		C.y = Idx - 16;
	}
	else if (Idx < 48)
	{
		// Bottom edge right -> left (including bottom-right corner at S0)
		C.x = 15 - (Idx - 32);
		C.y = 15;
	}
	else
	{
		// Left edge bottom -> top (including bottom-left corner at W0)
		C.x = 0;
		C.y = 15 - (Idx - 48);
	}

	return C;
}

static constexpr bool CanEnter(const Tile a_Tile, const EDirection a_Direction)
{
	return static_cast<uint8_t>(a_Tile) & (1u << (static_cast<uint8_t>(a_Direction) * 2 + 0));
}

static constexpr bool CanExit(const Tile a_Tile, const EDirection a_Direction)
{
	return static_cast<uint8_t>(a_Tile) & (1u << (static_cast<uint8_t>(a_Direction) * 2 + 1));
}

static constexpr void SetPortalExit(PortalVector& a_PortalVector, const EBorder a_Border)
{
	a_PortalVector |= (1ull << static_cast<uint8_t>(a_Border));
}

static constexpr bool GetPortalExit(const PortalVector a_PortalVector, const EBorder a_Border)
{
	return a_PortalVector & (1ull << static_cast<uint8_t>(a_Border));
}

static constexpr PortalVector GetExits(const Chunk& a_Chunk, const Coord a_Start)
{
	// Implement BFS algorithm that doesn't use heap memory.
	PortalVector portalVector = 0;
	PortalVector PortalVectorOut = 0;

	// Fixed-size ring buffer / queue for up to 16*16 tiles
	std::array<Coord, 16 * 16> Queue{};
	std::array<uint8_t, 16 * 16> Visited{};

	int Head = 0;
	int Tail = 0;

	// push start
	Queue[Tail++] = a_Start;
	Visited[static_cast<uint8_t>(a_Start.i)] = 1;

	constexpr int Dx[4] = { 0, 1, 0, -1 };
	constexpr int Dy[4] = { -1, 0, 1, 0 };

	while (Head != Tail)
	{
		const Coord Cur = Queue[Head++];
		const Tile CurTile = GetTile(a_Chunk, Cur);
		const int Cx = Cur.x;
		const int Cy = Cur.y;

		for (int DirIdx = 0; DirIdx < 4; ++DirIdx)
		{
			const EDirection Dir = static_cast<EDirection>(DirIdx);
			const EDirection Opp = static_cast<EDirection>((DirIdx + 2) & 3);
			const int Nx = Cx + Dx[DirIdx];
			const int Ny = Cy + Dy[DirIdx];

			// If moving out of bounds and this tile can exit that way, mark portal
			if (Nx < 0 || Nx > 15 || Ny < 0 || Ny > 15)
			{
				if (CanExit(CurTile, Dir))
				{
					// find the border index whose coord equals Cur
					for (int b = 0; b < 64; ++b)
					{
						if (GetCoord(static_cast<EBorder>(b)).i == Cur.i)
						{
							SetPortalExit(PortalVectorOut, static_cast<EBorder>(b));
							break;
						}
					}
				}
			}
			else
			{
				Coord N{};
				N.x = static_cast<int8_t>(Nx);
				N.y = static_cast<int8_t>(Ny);

				if (CanExit(CurTile, Dir) && CanEnter(GetTile(a_Chunk, N), Opp))
				{
					const uint8_t Idx = static_cast<uint8_t>(N.i);
					if (!Visited[Idx])
					{
						Visited[Idx] = 1;
						Queue[Tail++] = N;
					}
				}
			}
		}
	}

	return PortalVectorOut;
}

static std::vector<PortalVector> GetAllExits(const Chunk& a_Chunk)
{
	std::vector<PortalVector> AllExits;
	uint8_t i = 0;

	// For North side.
	for (; i < 16; ++i)
	{
		if (!CanEnter(GetTile(a_Chunk, GetCoord(static_cast<EBorder>(i))), EDirection::North))
		{
			AllExits.push_back(0);
		}
		else
		{
			AllExits.push_back(GetExits(a_Chunk, GetCoord(static_cast<EBorder>(i))));
		}
	}

	for (; i < 32; ++i)
	{
		if (!CanEnter(GetTile(a_Chunk, GetCoord(static_cast<EBorder>(i))), EDirection::East))
		{
			AllExits.push_back(0);
		}
		else
		{
			AllExits.push_back(GetExits(a_Chunk, GetCoord(static_cast<EBorder>(i))));
		}
	}

	for (; i < 48; ++i)
	{
		if (!CanEnter(GetTile(a_Chunk, GetCoord(static_cast<EBorder>(i))), EDirection::South))
		{
			AllExits.push_back(0);
		}
		else
		{
			AllExits.push_back(GetExits(a_Chunk, GetCoord(static_cast<EBorder>(i))));
		}
	}

	for (; i < 64; ++i)
	{
		if (!CanEnter(GetTile(a_Chunk, GetCoord(static_cast<EBorder>(i))), EDirection::West))
		{
			AllExits.push_back(0);
		}
		else
		{
			AllExits.push_back(GetExits(a_Chunk, GetCoord(static_cast<EBorder>(i))));
		}
	}

	return AllExits;
}

struct PortalInput
{
	EBorder Border;
	uint8_t Count;
	PortalVector Exits;
};

static std::vector<PortalInput> CollapsePortals(const std::vector<PortalVector>& a_Exits)
{
	std::vector<PortalInput> Collapsed;
	PortalInput Current{ .Border = EBorder::N0, .Count = 0, .Exits = 0 };

	for (size_t i = 0; i < a_Exits.size(); ++i)
	{
		if (a_Exits[i] == Current.Exits)
		{
			Current.Count++;
		}
		else
		{
			if (Current.Count > 0)
			{
				Collapsed.push_back(Current);
			}

			Current.Border = static_cast<EBorder>(i);
			Current.Count = 1;
			Current.Exits = a_Exits[i];
		}
	}

	if (Current.Count > 0)
	{
		Collapsed.push_back(Current);
	}

	// Handle ring wrap: if first and last entries have identical Exits, merge them
	if (Collapsed.size() > 1)
	{
		PortalInput& FirstRef = Collapsed.front();
		PortalInput& LastRef = Collapsed.back();

		if (FirstRef.Exits == LastRef.Exits)
		{
			// merge Last into First and adjust start border to Last.Border so the run wraps correctly
			FirstRef.Border = LastRef.Border;
			FirstRef.Count = static_cast<uint8_t>(FirstRef.Count + LastRef.Count);
			Collapsed.pop_back();
		}
	}

	return Collapsed;
}

//struct PortalLink
//{
//	int8_t PortalID;
//	EBorder Border;
//	uint8_t Count;
//	PortalVector Exits;
//};

// Test chunk where top half is open and bottom half is open but middle row 7 is DropSouth
constexpr Chunk TestChunk = {
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
	Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
};

void PrintPortalVector(const PortalVector a_PortalVector)
{
	// Print top border
	std::cout << "   ";
	for (int X = 0; X < 16; ++X)
	{
		for (int B = 0; B < 64; ++B)
		{
			const Coord C = GetCoord(static_cast<EBorder>(B));
			if (C.x == X && C.y == 0)
			{
				std::cout << (GetPortalExit(a_PortalVector, static_cast<EBorder>(B)) ? '1' : '0') << ' ';
				break;
			}
		}
	}
	std::cout << '\n';

	// Print rows with left/right border bits
	for (int Y = 0; Y < 16; ++Y)
	{
		// Left border bit for this row
		for (int B = 0; B < 64; ++B)
		{
			const Coord C = GetCoord(static_cast<EBorder>(B));
			if (C.x == 0 && C.y == Y)
			{
				std::cout << (GetPortalExit(a_PortalVector, static_cast<EBorder>(B)) ? '1' : '0') << ' ';
				break;
			}
		}

		// Interior of chunk
		for (int X = 0; X < 16; ++X)
		{
			std::cout << ". ";
		}

		// Right border bit for this row
		for (int B = 0; B < 64; ++B)
		{
			const Coord C = GetCoord(static_cast<EBorder>(B));
			if (C.x == 15 && C.y == Y)
			{
				std::cout << ' ' << (GetPortalExit(a_PortalVector, static_cast<EBorder>(B)) ? '1' : '0');
				break;
			}
		}

		std::cout << '\n';
	}

	// Print bottom border
	std::cout << "   ";
	for (int X = 0; X < 16; ++X)
	{
		for (int B = 0; B < 64; ++B)
		{
			const Coord C = GetCoord(static_cast<EBorder>(B));
			if (C.x == X && C.y == 15)
			{
				std::cout << (GetPortalExit(a_PortalVector, static_cast<EBorder>(B)) ? '1' : '0') << ' ';
				break;
			}
		}
	}
	std::cout << '\n';
}

int main()
{
	std::vector<PortalVector> AllExits = GetAllExits(TestChunk);
	std::vector<PortalInput> Collapsed = CollapsePortals(AllExits);
	


    return 0;
}