#pragma once

struct MyPlayer_t
{
	DWORD_PTR SomethingToPlayer1;
	DWORD_PTR SomethingToPlayer2;
	DWORD_PTR CLocalPlayer;
	Vector3 Position;
	float ViewAngle;

	void ReadInformation()
	{
		DWORD_PTR MainPtr = client->GetImage();
		DWORD_PTR MainPtrSize = client->GetSize();

		SomethingToPlayer1 = process->Read<DWORD>(MainPtr + 0x0132064C);
		SomethingToPlayer2 = process->Read<DWORD>(SomethingToPlayer1 + 0x2b0);

		Position = process->Read<Vector3>(SomethingToPlayer2 + 0xb8);
	}
}MyPlayer;

//Enemy struct
struct PlayerList_t
{
	DWORD EntityLoop;
	Vector3 Position;
	float ViewAngle;
	DWORD_PTR LoopDistance = 0x08;

	void ReadInformation(int Player)
	{
		DWORD_PTR MainPtr = client->GetImage();

		EntityLoop = process->Read<DWORD>(MainPtr + 0x0132064C + (Player * LoopDistance));
		
		Position = process->Read<Vector3>(EntityLoop + 0x3C);
		ViewAngle = process->Read<float>(EntityLoop + 0x3C + 0x10);

	}
}PlayerList[2024];