#pragma once

struct NetworkMessage
{
	enum class Type
	{
		invalidType,
		clientRegistration,
		clientAdded,
		replicateExistingClient,
		clientDisconnected,
		clientChangeUsername,
		clientChangeComputerName,
		clientSentMessage
	};
};
