#pragma once

struct NetworkMessage
{
	enum class Type
	{
		invalidType,
		clientRegistration,
		clientAdded,
		clientDisconnected,
		clientChangeUsername,
		clientChangeComputerName,
		clientSentMessage
	};
};
