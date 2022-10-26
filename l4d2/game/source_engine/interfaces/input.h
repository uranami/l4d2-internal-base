#pragma once

class CVerifiedUserCmd
{
public:
	CUserCmd  m_cmd;
	CRC32_t   m_crc;
};

struct IInput_t
{
	PAD(0xB1);
	bool m_fCameraInThirdPerson; //0x00B1
	PAD(0x2);
	Angle_t ThirdpersonCameraAngle; //0x00B4
	PAD(0x2C);
	CUserCmd* m_pCommands; //0x00EC
	CVerifiedUserCmd* m_pVerifiedCommands; //0x00F0
};