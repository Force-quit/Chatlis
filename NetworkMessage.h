#pragma once

struct NetworkMessage
{
	enum class Type
	{
		invalidType,
		clientRegistration,
		clientAdded,
		clientChangeUsername,
		clientChangeComputerName,
		clientSentMessage
	};
};
